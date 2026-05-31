//header file
#pragma once
#include "Mutator_base.h"

/**
 * Function_Call_Substitution_408
 */ 
class MutatorFrontendAction_408 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(408)

private:
    class MutatorASTConsumer_408 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_408(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        std::vector<const clang::CallExpr *> call_exprs;
    };
};

//source file
#include "../include/Function_Call_Substitution_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      call_exprs.push_back(CE);
    } else if (auto *FD =
                   Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncDecl")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getBeginLoc()))
        return;
      if (FD->isOverloadedOperator())
        return;
      if (FD->isMain())
        return;
      auto func_name = FD->getNameAsString();
      if (func_name == "")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "vsnprintf")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "putchar" || func_name == "fputc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fseek" || func_name == "ftell" ||
          func_name == "rewind" || func_name == "fgetpos" ||
          func_name == "fsetpos")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      if (func_name == "remove" || func_name == "rename" ||
          func_name == "tmpfile" || func_name == "tmpnam")
        return;
      if (func_name == "fgetpos" || func_name == "fsetpos")
        return;
      if (func_name == "ftell" || func_name == "rewind")
        return;
      if (func_name == "fseek")
        return;
      if (func_name == "fread" || func_name == "fwrite")
        return;
      if (func_name == "fclose" || func_name == "fopen" ||
          func_name == "freopen" || func_name == "setbuf" ||
          func_name == "setvbuf")
        return;
      if (func_name == "clearerr" || func_name == "feof" ||
          func_name == "ferror" || func_name == "fflush")
        return;
      if (func_name == "perror")
        return;
      if (func_name == "ungetc")
        return;
      if (func_name == "putc" || func_name == "getc")
        return;
      if (func_name == "getchar" || func_name == "fgetc")
        return;
      if (func_name == "gets" || func_name == "fgets")
        return;
      if (func_name == "puts" || func_name == "fputs")
        return;
      if (func_name == "getline" || func_name == "getdelim")
        return;
      if (func_name == "snprintf")
        return;
      if (func_name == "vprintf" || func_name == "vfprintf" ||
          func_name == "vsprintf")
        return;
      if (func_name == "vscanf" || func_name == "vfscanf" ||
          func_name == "vsscanf")
        return;
      if (func_name == "scanf" || func_name == "fscanf" ||
          func_name == "sscanf")
        return;
      if (func_name == "printf" || func_name == "fprintf" ||
          func_name == "sprintf")
        return;
      auto param_num = FD->getNumParams();
      if (param_num == 0)
        return;
      if (param_num > 2)
        return;
      llvm::outs() << func_name << '\n';
      if (param_num == 1) {
        auto param_type = FD->getParamDecl(0)->getType();
        if (!param_type->isPointerType())
          return;
      } else if (param_num == 2) {
        auto param_type1 = FD->getParamDecl(0)->getType();
        auto param_type2 = FD->getParamDecl(1)->getType();
        if (!param_type1->isPointerType() || !param_type2->isPointerType())
          return;
      }
      for (auto call : call_exprs) {
        if (call->getDirectCallee() != FD)
          continue;
        if (param_num == 1) {
          if (call->getNumArgs() != 1)
            continue;
          auto arg_type = call->getArg(0)->getType();
          if (!arg_type->isPointerType())
            continue;
          auto arg_content = stringutils::rangetoStr(
              *(Result.SourceManager), call->getArg(0)->getSourceRange());
          string replace_content = "/*mut408*/puts(" + arg_content + ")";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(call->getSourceRange()),
              replace_content);
        } else if (param_num == 2) {
          if (call->getNumArgs() != 2)
            continue;
          auto arg_type1 = call->getArg(0)->getType();
          auto arg_type2 = call->getArg(1)->getType();
          if (!arg_type1->isPointerType() || !arg_type2->isPointerType())
            continue;
          auto arg_content1 = stringutils::rangetoStr(
              *(Result.SourceManager), call->getArg(0)->getSourceRange());
          auto arg_content2 = stringutils::rangetoStr(
              *(Result.SourceManager), call->getArg(1)->getSourceRange());
          string replace_content =
              "/*mut408*/fputs(" + arg_content1 + "," + arg_content2 + ")";
          Rewrite.ReplaceText(
              CharSourceRange::getTokenRange(call->getSourceRange()),
              replace_content);
        }
      }
    }
}
  
void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto call_matcher = callExpr().bind("CallExpr");
    auto func_matcher = functionDecl().bind("FuncDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(call_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}