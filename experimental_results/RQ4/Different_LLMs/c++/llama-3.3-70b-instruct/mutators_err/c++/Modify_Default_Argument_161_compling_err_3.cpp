//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Default_Argument_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_161>(TheRewriter);
    }

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Default_Argument_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto params = FD->parameters();
      for (auto param : params) {
        if (param->hasDefaultArg()) {
          //Perform mutation on the source code text by applying string replacement
          auto defaultArg = param->getDefaultArg();
          auto paramName = param->getNameAsString();
          auto paramType = param->getType().getAsString();
          auto newDefaultArg = "g(" + paramName + ")";
          auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FD->getSourceRange());
          std::string defaultArgStr;
          clang::LangOptions lo;
          defaultArg->print(llvm::outs(), lo);
          llvm::outs().flush();
          defaultArgStr = llvm::outs().str();
          declaration.replace(declaration.find(defaultArgStr), defaultArgStr.length(), newDefaultArg);
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(hasDefaultArg(hasUnresolvedUses()))).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}