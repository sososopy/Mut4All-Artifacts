//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Parameters_In_Partial_Specialization_168
 */ 
class MutatorFrontendAction_168 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(168)

private:
    class MutatorASTConsumer_168 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_168(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_parameters_in_partial_specialization_168.h"

// ========================================================================================================
#define MUT168_OUTPUT 1

void MutatorFrontendAction_168::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TS = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!TS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TS->getLocation()))
        return;
      
      // Get the source code text of target node
      auto partialSpecText = stringutils::rangetoStr(*(Result.SourceManager), TS->getSourceRange());
      
      // Perform mutation by altering the parameters in the partial specialization
      std::string modifiedSpecText = partialSpecText;
      size_t pos = modifiedSpecText.find(">");
      if (pos != std::string::npos) {
          modifiedSpecText.insert(pos, ", typename U");
      }
      
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TS->getSourceRange()), modifiedSpecText);
    }
}
  
void MutatorFrontendAction_168::MutatorASTConsumer_168::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}