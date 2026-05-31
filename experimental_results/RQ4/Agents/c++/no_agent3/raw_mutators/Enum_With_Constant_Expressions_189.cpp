//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_With_Constant_Expressions_189
 */ 
class MutatorFrontendAction_189 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(189)

private:
    class MutatorASTConsumer_189 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_189(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/enum_with_constant_expressions_189.h"

// ========================================================================================================
#define MUT189_OUTPUT 1

void MutatorFrontendAction_189::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      // Ensure the enum has at least two enumerators
      if (ED->enumerator_begin() == ED->enumerator_end() || std::next(ED->enumerator_begin()) == ED->enumerator_end())
        return;

      // Get the source code text of target node
      auto enumSource = stringutils::rangetoStr(*(Result.SourceManager), ED->getSourceRange());

      // Choose an enumerator to modify
      auto enumIt = ED->enumerator_begin();
      std::advance(enumIt, getrandom::getRandomIndex(std::distance(ED->enumerator_begin(), ED->enumerator_end())));

      // Create a constant expression
      std::string constExpr = "1 + 2 * 3";

      // Perform mutation on the source code text by applying string replacement
      std::string originalEnum = (*enumIt)->getNameAsString();
      auto pos = enumSource.find(originalEnum);
      if (pos != std::string::npos) {
        enumSource.insert(pos + originalEnum.length(), " = " + constExpr);
      }

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), enumSource);
    }
}
  
void MutatorFrontendAction_189::MutatorASTConsumer_189::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl().bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}