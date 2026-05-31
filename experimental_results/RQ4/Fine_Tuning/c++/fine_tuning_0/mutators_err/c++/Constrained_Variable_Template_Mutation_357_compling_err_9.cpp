//header file
#pragma once
#include "Mutator_base.h"

/**
 * Constrained_Variable_Template_Mutation_357
 */ 
class MutatorFrontendAction_357 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(357)

private:
    class MutatorASTConsumer_357 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_357(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Constrained_Variable_Template_Mutation_357.h"

// ========================================================================================================
#define MUT357_OUTPUT 1

void MutatorFrontendAction_357::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("ConstrainedVarTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("requires") != string::npos) {
        int choice = getrandom::getRandomIndex(2);
        if (choice == 0) {
          if (declaration.find("&&") != string::npos)
            declaration.replace(declaration.find("&&"), 2, "||");
          else if (declaration.find("||") != string::npos)
            declaration.replace(declaration.find("||"), 2, "&&");
        } else if (choice == 1) {
          if (declaration.find("!") != string::npos)
            declaration.erase(declaration.find("!"), 1);
          else
            declaration.insert(declaration.find("requires") + 8, "!");
        } else if (choice == 2) {
          if (declaration.find("typename") != string::npos)
            declaration.replace(declaration.find("typename"), 8, "int");
          else if (declaration.find("int") != string::npos)
            declaration.replace(declaration.find("int"), 3, "typename");
        }
      } else {
        if (declaration.find("Test") != string::npos)
          declaration.replace(declaration.find("Test"), 4,
                              "typename T> requires(std::is_integral_v<T>)");
        else if (declaration.find("std::is_integral_v<T>") != string::npos)
          declaration.replace(declaration.find("std::is_integral_v<T>"), 22,
                              "Test");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_357::MutatorASTConsumer_357::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = clang::ast_matchers::varTemplateDecl().bind("ConstrainedVarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}