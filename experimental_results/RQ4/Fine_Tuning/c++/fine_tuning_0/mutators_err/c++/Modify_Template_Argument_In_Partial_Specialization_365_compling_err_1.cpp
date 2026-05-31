//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_In_Partial_Specialization_365
 */ 
class MutatorFrontendAction_365 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(365)

private:
    class MutatorASTConsumer_365 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_365(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Modify_Template_Argument_In_Partial_Specialization_365.h"

// ========================================================================================================
#define MUT365_OUTPUT 1

void MutatorFrontendAction_365::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("PartialSpecialization")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto specialization = stringutils::rangetoStr(
          *(Result.SourceManager), MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      specialization = std::regex_replace(specialization,
                                          std::regex("int"), "double");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), specialization);
    }
}
  
void MutatorFrontendAction_365::MutatorASTConsumer_365::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}