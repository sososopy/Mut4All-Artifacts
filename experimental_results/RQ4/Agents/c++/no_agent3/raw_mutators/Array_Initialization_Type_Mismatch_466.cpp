//header file
#pragma once
#include "Mutator_base.h"

/**
 * Array_Initialization_Type_Mismatch_466
 */ 
class MutatorFrontendAction_466 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(466)

private:
    class MutatorASTConsumer_466 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_466(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Array_Initialization_Type_Mismatch_466.h"

// ========================================================================================================
#define MUT466_OUTPUT 1

void MutatorFrontendAction_466::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitList = Result.Nodes.getNodeAs<clang::InitListExpr>("initList")) {
      //Filter nodes in header files
      if (!InitList || !Result.Context->getSourceManager().isWrittenInMainFile(
                     InitList->getBeginLoc()))
        return;
      
      for (unsigned i = 0; i < InitList->getNumInits(); ++i) {
          if (auto *Init = InitList->getInit(i)) {
              if (auto *CastExpr = llvm::dyn_cast<clang::CStyleCastExpr>(Init)) {
                  //Get the source code text of target node
                  auto TypeSourceInfo = CastExpr->getTypeInfoAsWritten();
                  auto OriginalType = TypeSourceInfo->getType().getAsString();
                  
                  //Perform mutation on the source code text by applying string replacement
                  std::string MutatedType = "float *"; // Example of type mismatch
                  std::string OriginalText = stringutils::rangetoStr(*(Result.SourceManager), CastExpr->getSourceRange());
                  std::string MutatedText = OriginalText;
                  size_t pos = MutatedText.find(OriginalType);
                  if (pos != std::string::npos) {
                      MutatedText.replace(pos, OriginalType.length(), MutatedType);
                  }
                  
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(CastExpr->getSourceRange()), MutatedText);
              }
          }
      }
    }
}
  
void MutatorFrontendAction_466::MutatorASTConsumer_466::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = initListExpr(hasType(arrayType(hasElementType(recordDecl())))).bind("initList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}