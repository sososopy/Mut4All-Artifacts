//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * CompoundLiteralEmbedModifier_393
 */ 
class MutatorFrontendAction_393 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(393)

private:
    class MutatorASTConsumer_393 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_393(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
        std::vector<std::string> filePaths;
        std::vector<std::string> types;
    };
};

//source file
#include "../include/CompoundLiteralEmbedModifier_393.h"

// ========================================================================================================
#define MUT393_OUTPUT 1

void MutatorFrontendAction_393::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *IL = Result.Nodes.getNodeAs<clang::InitListExpr>("InitList")) {
      //Filter nodes in header files
      if (!IL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     IL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto initList = stringutils::rangetoStr(*(Result.SourceManager), IL->getSourceRange());
      //Record the node information to be used in the mutation process
      for (auto child : IL->children()) {
        if (auto *SL = dyn_cast<clang::StringLiteral>(child)) {
          filePaths.push_back(SL->getString().str());
        }
      }
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        // Replace the #embed directive with a different file path
        std::string newFilePath = filePaths[getrandom::getRandomIndex(filePaths.size() - 1)];
        initList.replace(initList.find("#embed"), 7, "#embed \"" + newFilePath + "\"");
      } else {
        // Change the variable being initialized to a different type
        std::string newType = types[getrandom::getRandomIndex(types.size() - 1)];
        initList.replace(initList.find("char *"), 7, newType + " *");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(IL->getSourceRange()), initList);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getBeginLoc()))
        return;
      //Record the node information to be used in the mutation process
      types.push_back(VD->getType().getAsString());
    }
}
  
void MutatorFrontendAction_393::MutatorASTConsumer_393::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    using namespace clang::ast_matchers;
    auto matcher = initListExpr(hasDesc(stringLiteral())).bind("InitList");
    auto varDeclMatcher = varDecl().bind("VarDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(varDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}