```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Binary_Operator_477
 */ 
class MutatorFrontendAction_477 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(477)

private:
    class MutatorASTConsumer_477 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_477(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Binary_Operator_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *BO = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryOperator")) {
      //Filter nodes in header files
      if (!BO || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BO->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   BO->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string replacement;
      switch (BO->getOpcode()) {
        case BO_Add:
          replacement = "-";
          break;
        case BO_Sub:
          replacement = "+";
          break;
        case BO_Mul:
          replacement = "/";
          break;
        case BO_Div:
          replacement = "*";
          break;
        case BO_Rem:
          replacement = "%";
          break;
        case BO_EQ:
          replacement = "!=";
          break;
        case BO_NE:
          replacement = "==";
          break;
        case BO_LT:
          replacement = ">";
          break;
        case BO_GT:
          replacement = "<";
          break;
        case BO_LE:
          replacement = ">=";
          break;
        case BO_GE:
          replacement = "<=";
          break;
        case BO_And:
          replacement = "||";
          break;
        case BO_Or:
          replacement = "&&";
          break;
        default:
          return;
      }
      size_t pos = declaration.find(BO->getOpcodeStr().str());
      if (pos != std::string::npos) {
        declaration.replace(pos, BO->getOpcodeStr().size(), replacement);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(BO->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = binaryOperator().bind("BinaryOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}