```cpp
//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Unicode_Escape_Sequence_309
 */ 
class MutatorFrontendAction_309 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(309)

private:
    class MutatorASTConsumer_309 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_309(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Unicode_Escape_Sequence_309.h"

// ========================================================================================================
#define MUT309_OUTPUT 1

void MutatorFrontendAction_309::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SL = Result.Nodes.getNodeAs<clang::StringLiteral>(("StringLiteral"))) {
      //Filter nodes in header files
      if (!SL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SL->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   SL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string unicodeEscapeSequence = "\\u1234";
      size_t insertPosition = getrandom::getRandomIndex(declaration.length());
      declaration.insert(insertPosition, unicodeEscapeSequence);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SL->getSourceRange()), declaration);
    } else if (auto *CC = Result.Nodes.getNodeAs<clang::CharacterLiteral>(("CharacterLiteral"))) {
      //Filter nodes in header files
      if (!CC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CC->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   CC->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string unicodeEscapeSequence = "\\U12345678";
      size_t insertPosition = getrandom::getRandomIndex(declaration.length());
      declaration.insert(insertPosition, unicodeEscapeSequence);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CC->getSourceRange()), declaration);
    } else if (auto *C = Result.Nodes.getNodeAs<clang::Comment>(("Comment"))) {
      //Filter nodes in header files
      if (!C || !Result.Context->getSourceManager().isWrittenInMainFile(
                     C->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   C->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string unicodeEscapeSequence = "\\u1234";
      size_t insertPosition = getrandom::getRandomIndex(declaration.length());
      declaration.insert(insertPosition, unicodeEscapeSequence);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(C->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_309::MutatorASTConsumer_309::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = stringLiteral().bind("StringLiteral");
    DeclarationMatcher matcher2 = characterLiteral().bind("CharacterLiteral");
    DeclarationMatcher matcher3 = comment().bind("Comment");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
}