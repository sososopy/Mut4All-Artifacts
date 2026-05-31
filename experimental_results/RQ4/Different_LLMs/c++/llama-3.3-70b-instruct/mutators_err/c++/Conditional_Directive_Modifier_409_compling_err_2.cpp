//header file
#pragma once
#include "Mutator_base.h"

/**
 * Conditional_Directive_Modifier_409
 */ 
class MutatorFrontendAction_409 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_409>(TheRewriter);
    }

private:
    class MutatorASTConsumer_409 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_409(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Conditional_Directive_Modifier_409.h"

// ========================================================================================================
#define MUT409_OUTPUT 1

void MutatorFrontendAction_409::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::IfStmt>(("IfStmt"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto directive = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()),
          *Result.SourceManager, Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      int choice = rand() % 3;
      if (choice == 0) {
        // Invert the condition
        directive = directive.substr(0, directive.find("if")) + "if (!" + directive.substr(directive.find("if") + 2);
      } else if (choice == 1) {
        // Swap if and else
        size_t ifPos = directive.find("if");
        size_t elsePos = directive.find("else");
        if (elsePos != std::string::npos) {
          directive.replace(ifPos, 2, "el");
          directive.replace(elsePos, 4, "if");
        } else {
          directive.append("\nelse\n#endif");
        }
      } else {
        // Remove or add an else clause
        if (directive.find("else") != std::string::npos) {
          directive.erase(directive.find("else"), 4);
        } else {
          directive.append("\nelse\n#endif");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), directive);
    }
}

void MutatorFrontendAction_409::MutatorASTConsumer_409::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = ifStmt().bind("IfStmt");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}