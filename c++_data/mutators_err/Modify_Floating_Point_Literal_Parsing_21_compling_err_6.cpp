//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"

/**
 * Modify_Floating_Point_Literal_Parsing_21
 */ 
class MutatorFrontendAction_21 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(21)

private:
    class MutatorASTConsumer_21 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_21(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_floating_point_literal_parsing_21.h"

using namespace clang;
using namespace clang::ast_matchers;

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FL = Result.Nodes.getNodeAs<clang::FloatingLiteral>("FloatLiteral")) {
      //Filter nodes in header files
      if (!FL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FL->getLocation()))
        return;

      //Get the source code text of target node
      auto floatText = Lexer::getSourceText(CharSourceRange::getTokenRange(FL->getSourceRange()), 
                                            *Result.SourceManager, 
                                            Result.Context->getLangOpts());

      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (llvm::StringRef(floatText).endswith(".0")) {
        mutatedText = (llvm::Twine(floatText.substr(0, floatText.size() - 2)) + ".0f").str();
      } else {
        return;
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(FL->getSourceRange(), mutatedText);
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = floatingLiteral().bind("FloatLiteral");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}