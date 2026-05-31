//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_template_condition_in_static_function_48
 */ 
class MutatorFrontendAction_48 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(48)

private:
    class MutatorASTConsumer_48 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_48(Rewriter &R) : TheRewriter(R) {}
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
#include "Mutator_base.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TemplateCondition")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getTemplateNameLoc()))
        return;
      
      //Get the source code text of target node
      auto condition = MT->getTemplateArgs().get(0).getAsType().getAsString();
      auto mutatedCondition = condition;
      
      //Perform mutation on the source code text by applying string replacement
      if (mutatedCondition.find("::") != std::string::npos) {
        mutatedCondition.insert(mutatedCondition.find_last_of(":"), "nonExistentMember");
      }

      //Replace the original AST node with the mutated one
      auto srcRange = MT->getSourceRange();
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(srcRange), mutatedCondition);
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(cxxMethodDecl(isStaticStorageClass(), hasDescendant(templateSpecializationType()))
        )).bind("TemplateCondition");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}