//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Requirements_316
 */ 
class MutatorFrontendAction_316 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(316)

private:
    class MutatorASTConsumer_316 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_316(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_requirements_316.h"

// ========================================================================================================
#define MUT316_OUTPUT 1

void MutatorFrontendAction_316::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("TemplateConstructor")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      if (CT->getNumParams() != 1)
        return;

      auto *T = CT->getTemplateInstantiationPattern();
      if (!T)
        return;

      std::string originalCode = stringutils::rangetoStr(*(Result.SourceManager), CT->getSourceRange());

      size_t requiresPos = originalCode.find("requires requires");
      if (requiresPos != std::string::npos) {
        std::string mutatedCode = originalCode;
        mutatedCode.insert(requiresPos, "requires requires(Y y) { a.x = y; }; ");
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CT->getSourceRange()), mutatedCode);
      }
    }
}
  
void MutatorFrontendAction_316::MutatorASTConsumer_316::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        hasParent(cxxRecordDecl(isStruct(), hasTemplateArgument(
            templateArgument().bind("TemplateArgument"))))
    ).bind("TemplateConstructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}