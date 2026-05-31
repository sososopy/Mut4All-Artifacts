//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Modify_Initializer_List_301
 */ 
class MutatorFrontendAction_301 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(301)

private:
    class MutatorASTConsumer_301 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_301(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Initializer_List_301.h"

// ========================================================================================================
#define MUT301_OUTPUT 1

void MutatorFrontendAction_301::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VarDecl = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VarDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VarDecl->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = VarDecl->getInit();
      if (!initList)
        return;
      auto initListRange = initList->getSourceRange();
      auto initListText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(initListRange), *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      // Change the type of an initializer to a different type that already exists within the program
      // or add a new initializer of a type that exists in the program, or remove an initializer
      std::vector<std::string> types;
      for (auto child : initList->children()) {
        if (auto init = clang::dyn_cast<clang::InitListExpr>(child)) {
          for (auto initChild : init->children()) {
            if (auto expr = clang::dyn_cast<clang::Expr>(initChild)) {
              if (auto type = expr->getType().getAsString()) {
                types.push_back(type);
              }
            }
          }
        }
      }
      if (!types.empty()) {
        std::string newType = types[0];
        std::string mutatedInitListText = std::string(initListText);
        // Replace the first initializer with a new type
        size_t pos = mutatedInitListText.find_first_of("{");
        if (pos != std::string::npos) {
          mutatedInitListText.replace(pos + 1, mutatedInitListText.find_first_of(",") - pos - 1, newType + "()");
        }
        // Add a new initializer of the new type
        mutatedInitListText += ", " + newType + "()";
        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(initListRange), mutatedInitListText);
      }
    }
}
  
void MutatorFrontendAction_301::MutatorASTConsumer_301::HandleTranslationUnit(ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = clang::ast_matchers::varDecl(clang::ast_matchers::hasInitializer(clang::ast_matchers::initListExpr())).bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}