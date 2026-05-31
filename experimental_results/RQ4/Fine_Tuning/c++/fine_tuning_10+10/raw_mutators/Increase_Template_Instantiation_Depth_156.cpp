//header file
#pragma once
#include "Mutator_base.h"

/**
 * Increase_Template_Instantiation_Depth_156
 */ 
class MutatorFrontendAction_156 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(156)

private:
    class MutatorASTConsumer_156 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_156(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Increase_Template_Instantiation_Depth_156.h"

// ========================================================================================================
#define MUT156_OUTPUT 1

void MutatorFrontendAction_156::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FT->getLocation()))
        return;

      if (FT->getTemplatedDecl()->hasBody()) {
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FT->getTemplatedDecl()->getSourceRange());
        size_t pos = declaration.find("{");
        if (pos != std::string::npos) {
          declaration.insert(pos + 1, "\nif (value % 2 == 0) return value + recursiveFunction(value - 1) + recursiveFunction(value - 2);\n");
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(FT->getTemplatedDecl()->getSourceRange()), declaration);
        }
      }
    }
}
  
void MutatorFrontendAction_156::MutatorASTConsumer_156::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(ifStmt())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}