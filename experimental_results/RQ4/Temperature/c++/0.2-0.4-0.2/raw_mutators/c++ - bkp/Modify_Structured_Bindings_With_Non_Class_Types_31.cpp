//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_structured_bindings_with_non_class_types_31
 */ 
class MutatorFrontendAction_31 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(31)

private:
    class MutatorASTConsumer_31 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_31(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_structured_bindings_with_non_class_types_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("BindingDecl")) {
      //Filter nodes in header files
      if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BD->getLocation()))
        return;

      const auto &Bindings = BD->bindings();
      if (Bindings.size() == 2) {
        //Get the source code text of target node
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), BD->getSourceRange());
        std::string newText = "const auto& " + Bindings[0]->getNameAsString() + " = " + BD->getInit()->getSourceRange().printToString(*(Result.SourceManager)) + ";";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(BD->getSourceRange()), newText);
      }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = decompositionDecl(isInTemplateInstantiation()).bind("BindingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}