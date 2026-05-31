//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * NestedArrayInitLoopExprModification_476
 */ 
class MutatorFrontendAction_476 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(476)

private:
    class MutatorASTConsumer_476 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_476(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NestedArrayInitLoopExprModification_476.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/InitListExpr.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Basic/SourceManager.h"

// ========================================================================================================
#define MUT476_OUTPUT 1

void MutatorFrontendAction_476::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::InitListExpr>("InitListExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Change the order of elements
      size_t pos = declaration.find('{');
      size_t endPos = declaration.find('}');
      std::string elements = declaration.substr(pos + 1, endPos - pos - 1);
      std::vector<std::string> elementList;
      size_t start = 0;
      while (true) {
        size_t commaPos = elements.find(',', start);
        if (commaPos == std::string::npos) {
          elementList.push_back(elements.substr(start));
          break;
        }
        elementList.push_back(elements.substr(start, commaPos - start));
        start = commaPos + 1;
      }
      std::random_shuffle(elementList.begin(), elementList.end());
      std::string mutatedElements;
      for (const auto &element : elementList) {
        mutatedElements += element + ",";
      }
      mutatedElements.pop_back(); // Remove the last comma
      declaration.replace(pos + 1, endPos - pos - 1, mutatedElements);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_476::MutatorASTConsumer_476::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    using namespace clang::ast_matchers;
    ExprMatcher matcher = initListExpr().bind("InitListExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}