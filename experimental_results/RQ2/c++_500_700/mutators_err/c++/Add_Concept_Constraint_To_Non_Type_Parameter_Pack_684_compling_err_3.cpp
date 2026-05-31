//header file
#pragma once
#include "Mutator_base.h"

/**
 * add_concept_constraint_to_non_type_parameter_pack_684
 */ 
class MutatorFrontendAction_684 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(684)

private:
    class MutatorASTConsumer_684 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_684(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_concept_constraint_to_non_type_parameter_pack_684.h"

// ========================================================================================================
#define MUT684_OUTPUT 1

void MutatorFrontendAction_684::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());

      std::string conceptDef = "template<class T> concept MyConcept = requires(T a) { /* some valid expression involving a */ };";
      std::string newTemplate = "template<MyConcept auto... args>";

      size_t pos = declaration.find("template<auto... args>");
      if (pos != std::string::npos) {
          declaration.replace(pos, 21, newTemplate);
      }

      declaration = conceptDef + "\n" + declaration;

      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_684::MutatorASTConsumer_684::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(parameterCountIs(1))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}