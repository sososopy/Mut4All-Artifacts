//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Requires_With_Other_Keywords_116
 */ 
class MutatorFrontendAction_116 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(116)

private:
    class MutatorASTConsumer_116 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_116(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_requires_with_other_keywords_116.h"

// ========================================================================================================
#define MUT116_OUTPUT 1

void MutatorFrontendAction_116::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RequiresExpr>("RequiresExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto requiresExpr = stringutils::rangetoStr(*(Result.SourceManager),
                                                  MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::vector<string> keywords = {
          "auto",     "int",       "struct",     "namespace", "if",
          "while",    "for",       "switch",     "class",     "case",
          "default",  "enum",      "else",       "do",        "typedef",
          "void",     "catch",     "template",   "return",    "goto",
          "break",    "continue",  "dynamic_cast", "if constexpr", "this",
          "asm",      "try",       "using",      "sizeof",    "constexpr",
          "throw",    "alignas",   "alignof",    "concept",   "consteval",
          "decltype",
      };
      int index = getrandom::getRandomIndex(keywords.size() - 1);
      llvm::outs() << keywords[index] << '\n';
      llvm::outs() << requiresExpr << '\n';
      stringutils::replaceFirst(requiresExpr, "requires", keywords[index]);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()),
                          requiresExpr);
    }
}
  
void MutatorFrontendAction_116::MutatorASTConsumer_116::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = requiresExpr().bind("RequiresExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}