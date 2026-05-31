//header file
#pragma once
#include "Mutator_base.h"

/**
 * Exception_Specification_Mutation_In_Function_Templates_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)

private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
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
        
    };
};

//source file
#include "../include/Exception_Specification_Mutation_In_Function_Templates_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (FT->getTemplatedDecl()->hasDynamicExceptionSpec()) {
        auto pos = declaration.find("throw()");
        if (pos != string::npos) {
          declaration.replace(pos, 7, "");
        }
      } else if (FT->getTemplatedDecl()->hasNoThrowAttr()) {
        auto pos = declaration.find("noexcept");
        if (pos != string::npos) {
          declaration.replace(pos, 8, "throw()");
        }
      } else {
        auto pos = declaration.find("{");
        if (pos != string::npos) {
          declaration.insert(pos, "noexcept ");
        }
      }
      llvm::outs() << declaration << '\n';
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}