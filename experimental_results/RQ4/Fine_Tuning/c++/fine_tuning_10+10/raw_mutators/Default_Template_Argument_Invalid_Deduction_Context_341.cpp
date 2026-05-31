//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Template_Argument_Invalid_Deduction_Context_341
 */ 
class MutatorFrontendAction_341 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(341)

private:
    class MutatorASTConsumer_341 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_341(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Default_Template_Argument_Invalid_Deduction_Context_341.h"

// ========================================================================================================
#define MUT341_OUTPUT 1

void MutatorFrontendAction_341::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (TD->getTemplatedDecl()->hasDefinition()) {
        auto *CXXRD = TD->getTemplatedDecl();
        std::string className = CXXRD->getNameAsString();
        std::string mutation = "template<typename T, bool = sizeof(T) < sizeof(T)> class " + className + " { " + className + "(); };";
        SourceLocation startLoc = TD->getBeginLoc();
        SourceLocation endLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), mutation);
      }
    }
}
  
void MutatorFrontendAction_341::MutatorASTConsumer_341::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxRecordDecl(has(cxxConstructorDecl())))).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}