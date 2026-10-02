#!/usr/bin/env node
// Cline's real SDK/runtime and existing login; exactly six domain tools.
import {spawn} from 'node:child_process';
import {createInterface} from 'node:readline';
import {readFileSync,writeFileSync,renameSync,existsSync} from 'node:fs';
import {dirname,join} from 'node:path';
import {homedir} from 'node:os';
import {fileURLToPath,pathToFileURL} from 'node:url';
const root=dirname(dirname(fileURLToPath(import.meta.url)));
const sdkPath=join(homedir(),'.cache/fzgx-agents/cline-sdk/node_modules/@cline/sdk/dist/index.js');
const {Agent,createTool,ProviderSettingsManager,getProviderAuthHandler,getProviderOAuthCredentialsFromSettings}=await import(pathToFileURL(sdkPath));
const manager=new ProviderSettingsManager();
let config=manager.getProviderConfig('cline');
if(config && process.env.FZGX_MODEL) config.modelId=process.env.FZGX_MODEL;
if(!config?.apiKey) throw new Error('Existing Cline login is unavailable; no interactive auth attempted');
const names=['write_unit','patch_unit','check','search','read_evidence','release'];
// 0 disables a guard, as in codex_server.
const tokenLimits={input:Number(process.env.FZGX_MAX_MODEL_INPUT_TOKENS||0),output:Number(process.env.FZGX_MAX_MODEL_OUTPUT_TOKENS||0)};
if(process.argv.includes('--describe')){
 console.log(JSON.stringify({harness:'cline-sdk',provider:config.providerId,model:config.modelId,effort:'high',tools:names,nativeTools:[],modelTools:[],tokenLimits}));
 process.exit(0);
}
const settings=manager.getProviderSettings(config.providerId);
const handler=getProviderAuthHandler(config.providerId);
const credentials=settings && getProviderOAuthCredentialsFromSettings(config.providerId,settings);
if(handler && credentials){
 const updated=await handler.refresh({settings,credentials});
 if(!updated) throw new Error('Cline login refresh rejected; no interactive auth attempted');
 if(updated.access!==credentials.access) handler.saveCredentials({manager,settings,credentials:updated,setLastUsed:false,save:true});
 config=manager.getProviderConfig(config.providerId);
 if(process.env.FZGX_MODEL) config.modelId=process.env.FZGX_MODEL;
}
const symbol=process.env.FZGX_SYMBOL, identity=process.env.FZGX_AGENT_ID;
const terminal=process.env.FZGX_RESULT_FILE;
if(!symbol || !identity || !terminal || !terminal.endsWith('/'+symbol+'.terminal.json')) throw new Error('Host-bound assignment required');
const directory=dirname(terminal);
const prompt=readFileSync(process.argv[2],'utf8');
const env=Object.fromEntries(['FZGX_SYMBOL','FZGX_AGENT_ID','FZGX_HARNESS','FZGX_MODEL','FZGX_RESULT_FILE'].map(k=>[k,process.env[k]]));
const worker=spawn(join(root,'.venv/bin/python'),[join(root,'tools/fzgx.py'),'--tool-worker'],{cwd:root,stdio:['pipe','pipe','inherit']});
const replies=[];
createInterface({input:worker.stdout}).on('line',line=>{
 const pending=replies.shift(); if(pending) {try{pending.resolve(JSON.parse(line));}catch(e){pending.reject(e);}}
});
worker.on('exit',code=>{for(const pending of replies.splice(0)) pending.reject(new Error('Bound tool worker exited '+code));});
const call=argv=>new Promise((resolve,reject)=>{
 replies.push({resolve,reject}); worker.stdin.write(JSON.stringify({args:['--json',...argv],env})+'\n');
});
const event=(type,data={})=>console.log(JSON.stringify({timestamp:Date.now()/1000,type,...data}));
let agent;
const fields={write_unit:{source:'Complete C source'},patch_unit:{old:'Unique exact text',new:'Replacement text'},check:{versions:'Empty for current compiler, all, or comma-separated versions'},search:{},read_evidence:{section:'diff or data',cursor:'0 or next cursor'},release:{reason:'Precise technical obstacle'}};
const descriptions={write_unit:'Replace the complete assigned C source, compile and diff against retail',patch_unit:'Replace one unique substring, compile and diff',check:'Check current source or probe compilers retaining the best',search:'Let the tooling permute your current source mechanically (declaration order, type/sign flips, pragmas, pool priming); a match is accepted, a better body becomes your work copy; use at 80%+ when register, pool or L rows remain; three uses per attempt',read_evidence:'Read cached diff or proven retail data without compiling',release:'Save best candidate and stop'};
const tools=names.map(name=>createTool({name,description:descriptions[name],inputSchema:{type:'object',properties:Object.fromEntries(Object.entries(fields[name]).map(([k,v])=>[k,{type:'string',description:v,maxLength:32768}])),required:Object.keys(fields[name]),additionalProperties:false},async execute(input){
 if(Object.keys(input).some(k=>!(k in fields[name]))) throw new Error('Unexpected tool argument');
 event('tool-started',{tool:name}); let paths=[];
 try{
  let argv=[];
  if(name==='write_unit'||name==='patch_unit'){
   argv=[name.replaceAll('_','-'),symbol,'--agent',identity];
   for(const [key,value] of Object.entries(input)){
    const path=join(directory,symbol+'.tool.'+key);writeFileSync(path,value);paths.push(path);
    argv.push(key==='source'?'--file':'--'+key+'-file',path);
   }
  }else if(name==='check') argv=['check',symbol,...(input.versions?['--versions',input.versions]:[])];
  else if(name==='search') argv=['search',symbol,'--agent',identity];
  else if(name==='read_evidence') argv=['read-evidence',symbol,'--section',input.section,'--cursor',input.cursor];
  else argv=['release',symbol,'--agent',identity,'--reason',input.reason];
  const response=await call(argv);
  if(response.rc!==0 && !response.stdout) throw new Error((response.stderr||'Domain tool failed').slice(-2000));
  const result=JSON.parse(response.stdout);
  event('tool-finished',{tool:name,ok:result.ok!==false});
  if(existsSync(terminal)) agent.abort('Terminal function outcome; no summary model call');
  return result;
 }finally{const fs=await import('node:fs');for(const p of paths) fs.unlinkSync(p);}
}}));
agent=new Agent({providerId:config.providerId,modelId:config.modelId,apiKey:config.apiKey,baseUrl:config.baseUrl,
 options:{...config,reasoningEffort:'high'},systemPrompt:readFileSync(join(root,'tools/codex_matcher.md'),'utf8'),tools,modelTools:[],
 maxIterations:40,toolExecution:'sequential',modelOptions:{maxTokens:32768,reasoningEffort:'high'},
 requestToolApproval:request=>({approved:names.includes(request.toolName)}),
});
let usage={inputTokens:0,outputTokens:0,cacheReadTokens:0,cacheWriteTokens:0,totalCost:0};
let budgetReached=false;
agent.subscribe(e=>{
 if(e.type==='usage-updated'){
  // The SDK reports cache counters on this same event. Dropping them made every
  // cline row look 0% cached, because nothing downstream could see a rate the
  // harness never recorded (see docs/findings/276). inputTokens is cache-INCLUSIVE
  // (turn N reads exactly turn N-1's inputTokens), so cacheReadTokens is a subset
  // of it and must never be added on top; effective = input - 0.9*cacheRead.
  usage={inputTokens:e.usage.inputTokens||0,outputTokens:e.usage.outputTokens||0,
         cacheReadTokens:e.usage.cacheReadTokens||0,cacheWriteTokens:e.usage.cacheWriteTokens||0,
         totalCost:e.usage.totalCost||0};
  const path=join(directory,symbol+'.usage.json');writeFileSync(path+'.tmp',JSON.stringify(usage));renameSync(path+'.tmp',path);
  event(e.type,{usage});
  if((tokenLimits.input&&usage.inputTokens>=tokenLimits.input)||(tokenLimits.output&&usage.outputTokens>=tokenLimits.output)){budgetReached=true;agent.abort('Function emergency token budget reached; preserve only this candidate');}
 }else if(!e.type.endsWith('-delta') && e.type!=='message-added' && e.type!=='assistant-message') event(e.type);
});
for(const sig of ['SIGINT','SIGTERM']) process.on(sig,()=>agent.abort('Host shutdown; drain domain tools'));
try{
 event('system',{harness:'cline-sdk',model:config.modelId,tools:names,nativeTools:[],tokenLimits});
 const result=await agent.run(prompt);
 event('result',{model:config.modelId,status:result.status,usage,text:result.outputText?.slice(-2000)});
 process.exitCode=result.status==='failed'?1:0;
}catch(error){
 if(!existsSync(terminal)&&!budgetReached){event('error',{message:String(error.message).slice(0,2000)});process.exitCode=1;}
 else event('result',{model:config.modelId,status:'terminal',usage});
}finally{
try{
 if(budgetReached && !existsSync(terminal)){
  const response=await call(['release',symbol,'--agent',identity,'--save-only','--reason','Per-function emergency token budget reached; preserve best candidate without stopping sibling workers']);
  const result=JSON.parse(response.stdout||'{}');
  if(response.rc!==0||result.ok===false||!existsSync(terminal)){event('error',{message:'Budget release failed'});process.exitCode=1;}
  else {event('result',{model:config.modelId,status:'budget-released',usage});process.exitCode=0;}
 }
}catch(error){event('error',{message:'Budget release failed: '+String(error.message).slice(0,1000)});process.exitCode=1;}
finally{worker.stdin.end();await new Promise(resolve=>worker.once('exit',resolve));}
}
