//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Builtin_Function_Argument_List_206
 */ 
class MutatorFrontendAction_206 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(206)

private:
    class MutatorASTConsumer_206 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_206(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_builtin_function_argument_list_206.h"

// ========================================================================================================
#define MUT206_OUTPUT 1

void MutatorFrontendAction_206::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("builtinCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;

      //Get the source code text of target node
      auto sourceRange = MT->getSourceRange();
      auto originalCall = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCall = originalCall;
      size_t pos = mutatedCall.find("(");
      if (pos != std::string::npos) {
          mutatedCall.insert(pos + 1, "\"0\", ");
          size_t lastComma = mutatedCall.rfind(",");
          if (lastComma != std::string::npos) {
              mutatedCall.replace(lastComma + 1, std::string::npos, " 42, (x))");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedCall);
    }
}

void MutatorFrontendAction_206::MutatorASTConsumer_206::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(callee(functionDecl(hasName("__builtin_fpclassify")))).bind("builtinCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}