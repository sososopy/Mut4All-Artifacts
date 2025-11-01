//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrained_Inner_Class_Template_Mutation_87
 */ 
class MutatorFrontendAction_87 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(87)

private:
    class MutatorASTConsumer_87 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_87(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Constrained_Inner_Class_Template_Mutation_87.h"

// ========================================================================================================
#define MUT87_OUTPUT 1

void MutatorFrontendAction_87::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InnerTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("InnerTemplate")) {
      //Filter nodes in header files
      if (!InnerTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(
                     InnerTemplate->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 InnerTemplate->getSourceRange());
      
      //Perform mutation on the source code text by applying string replacement
      size_t pos = declaration.find("requires");
      if (pos != std::string::npos) {
          declaration.insert(pos + 8, " AdditionalConstraint<U> &&");
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(InnerTemplate->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_87::MutatorASTConsumer_87::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(
        hasName("Inner"),
        hasTemplateParameter(0, hasTypeTemplateParameter(hasType(hasConstraint(anything()))))
    ).bind("InnerTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}