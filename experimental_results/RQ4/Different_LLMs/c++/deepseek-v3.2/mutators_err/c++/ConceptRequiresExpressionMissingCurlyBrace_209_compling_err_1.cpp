//header file
#pragma once
#include "Mutator_base.h"

/**
 * ConceptRequiresExpressionMissingCurlyBrace_209
 */ 
class MutatorFrontendAction_209 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(209)

private:
    class MutatorASTConsumer_209 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_209(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_ConceptRequiresExpressionMissingCurlyBrace_209.h"

// ========================================================================================================
#define MUT209_OUTPUT 1

void MutatorFrontendAction_209::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto conceptText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Find requires-expression in the concept definition
      size_t requiresPos = conceptText.find("requires");
      if (requiresPos == std::string::npos)
        return;
      //Find the opening brace of the requirement body
      size_t bodyStart = conceptText.find('{', requiresPos);
      if (bodyStart == std::string::npos)
        return;
      //Find the closing brace of the requirement body
      size_t bodyEnd = conceptText.find('}', bodyStart + 1);
      if (bodyEnd == std::string::npos)
        return;
      //Ensure there's at least one requirement inside
      std::string bodyContent = conceptText.substr(bodyStart + 1, bodyEnd - bodyStart - 1);
      if (bodyContent.find_first_not_of(" \t\n") == std::string::npos)
        return;
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      std::string mutatedText;
      if (choice == 0) {
        // Replace closing brace with semicolon
        mutatedText = conceptText.substr(0, bodyEnd) + " ;" + conceptText.substr(bodyEnd + 1);
      } else {
        // Omit closing brace entirely
        mutatedText = conceptText.substr(0, bodyEnd) + conceptText.substr(bodyEnd + 1);
      }
      mutatedText = "/*mut209*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_209::MutatorASTConsumer_209::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}