//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Consteval_Constructor_69
 */ 
class MutatorFrontendAction_69 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(69)

private:
    class MutatorASTConsumer_69 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_69(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_consteval_constructor_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constevalCtor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Perform mutation on the source code text by applying string replacement
      if (MT->isConsteval()) {
        SourceLocation startLoc = MT->getBeginLoc();
        SourceLocation endLoc = MT->getEndLoc();
        std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();
        std::string mutatedText = originalText;
        size_t pos = mutatedText.find("consteval");
        if (pos != std::string::npos) {
          mutatedText.replace(pos, 9, "constexpr");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isConsteval()).bind("constevalCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}