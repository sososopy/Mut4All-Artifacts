//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Concept_With_Template_Template_Parameters_103
 */ 
class MutatorFrontendAction_103 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(103)

private:
    class MutatorASTConsumer_103 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_103(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_concept_with_template_template_parameters_103.h"

// ========================================================================================================
#define MUT103_OUTPUT 1

void MutatorFrontendAction_103::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionTemplate")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->getTemplateSpecializationKind() != TSK_Undeclared)
        return;

      if (auto *CTD = FD->getDescribedFunctionTemplate()) {
        if (CTD->getTemplateParameters()->size() > 0) {
          auto conceptDecl = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl");
          if (!conceptDecl)
            return;

          auto conceptSourceRange = conceptDecl->getSourceRange();
          std::string conceptSource = clang::Lexer::getSourceText(
              clang::CharSourceRange::getTokenRange(conceptSourceRange),
              *Result.SourceManager, Result.Context->getLangOpts()).str();

          std::string modifiedConceptSource = conceptSource;
          size_t pos = modifiedConceptSource.find("template <class>");
          if (pos != std::string::npos) {
            modifiedConceptSource.replace(pos, 16, "typename");
          }

          Rewrite.ReplaceText(conceptSourceRange, modifiedConceptSource);
        }
      }
    }
}
  
void MutatorFrontendAction_103::MutatorASTConsumer_103::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionTemplateMatcher = functionDecl(
        isTemplateInstantiation(), 
        hasDescendant(clang::conceptDecl().bind("ConceptDecl"))
    ).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionTemplateMatcher, &callback);
    matchFinder.matchAST(Context);
}