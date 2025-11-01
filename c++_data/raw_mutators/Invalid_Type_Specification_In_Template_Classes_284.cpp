//header file
#pragma once
#include "Mutator_base.h"

/**
 * Invalid_Type_Specification_In_Template_Classes_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)

private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Invalid_Type_Specification_In_Template_Classes_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TC = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      if (!TC || !Result.Context->getSourceManager().isWrittenInMainFile(TC->getLocation()))
        return;

      auto *CXXRD = TC->getTemplatedDecl();
      if (!CXXRD || !CXXRD->isCompleteDefinition())
        return;

      std::string classText = stringutils::rangetoStr(*(Result.SourceManager), CXXRD->getSourceRange());
      std::string newTemplateParam = ", typename U";
      std::string newFunction = "\nvoid dummyFunction() { U dummyVar; }\n";

      size_t templatePos = classText.find("template <");
      size_t classBodyPos = classText.find("{", templatePos);
      if (templatePos != std::string::npos && classBodyPos != std::string::npos) {
        classText.insert(templatePos + std::string("template <typename T").length(), newTemplateParam);
        classText.insert(classBodyPos + 1, newFunction);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(CXXRD->getSourceRange()), classText);
      }
    }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}