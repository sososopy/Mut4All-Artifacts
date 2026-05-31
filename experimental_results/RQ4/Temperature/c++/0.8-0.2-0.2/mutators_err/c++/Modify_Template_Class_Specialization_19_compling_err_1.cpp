//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_class_specialization_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)

private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_template_class_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Spec = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpec")) {
      //Filter nodes in header files
      if (!Spec || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Spec->getLocation()))
        return;
      
      //Get the source code text of target node
      auto specSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                Spec->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string modifiedSpec = specSource;
      modifiedSpec = modifiedSpec.replace(modifiedSpec.find("typename"), 7, "typename T");
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(Spec->getSourceRange()), modifiedSpec);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpec");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}