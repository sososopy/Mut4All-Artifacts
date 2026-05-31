//header file
#pragma once
#include "Mutator_base.h"

/**
 * Partial_Specialization_Violation_In_Template_Structures_191
 */ 
class MutatorFrontendAction_191 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(191)

private:
    class MutatorASTConsumer_191 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_191(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::ClassTemplatePartialSpecializationDecl *> cur_partial_specializations;
    };
};

//source file
#include "../include/Partial_Specialization_Violation_In_Template_Structures_191.h"

// ========================================================================================================
#define MUT191_OUTPUT 1

void MutatorFrontendAction_191::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *PS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecializations")) {
      //Filter nodes in header files
      if (!PS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     PS->getLocation()))
        return;
      //Get the source code text of target node
      auto partial_specialization = stringutils::rangetoStr(*(Result.SourceManager), PS->getSourceRange());
      //Record the node information of the partial specialization
      cur_partial_specializations.push_back(PS);
      //Perform mutation on the source code text by applying string replacement
      partial_specialization = partial_specialization.substr(partial_specialization.find('>') + 1);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(PS->getSourceRange()), partial_specialization);
    }
}

void MutatorFrontendAction_191::MutatorASTConsumer_191::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    auto partial_specialization_matcher = classTemplatePartialSpecializationDecl().bind("PartialSpecializations");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(partial_specialization_matcher, &callback);
    matchFinder.matchAST(Context);
}