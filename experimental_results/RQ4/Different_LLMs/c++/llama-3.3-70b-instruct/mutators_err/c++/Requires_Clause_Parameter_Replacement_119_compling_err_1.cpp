//header file
#pragma once
#include "Mutator_base.h"

/**
 * Requires_Clause_Parameter_Replacement_119
 */ 
class MutatorFrontendAction_119 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(119)

private:
    class MutatorASTConsumer_119 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_119(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Requires_Clause_Parameter_Replacement_119.h"

// ========================================================================================================
#define MUT119_OUTPUT 1

void MutatorFrontendAction_119::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>(("RequiresExpr"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the parameter value accessed within the requires clause
      // with a different variable or expression that is already defined in the seed program
      // For example, given a template function `test` with a requires clause 
      // `requires std::assignable_from<Type&, decltype(std::forward<O>(value))>`, 
      // this operator could replace `value` with another variable, such as `target`, 
      // resulting in `requires std::assignable_from<Type&, decltype(std::forward<O>(target))>`
      std::string replacement = "target";
      size_t pos = declaration.find("value");
      if (pos != std::string::npos) {
        declaration.replace(pos, 5, replacement);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_119::MutatorASTConsumer_119::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    ExprMatcher matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}