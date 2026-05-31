//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Using_Declaration_In_Class_Scope_327
 */ 
class MutatorFrontendAction_327 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(327)

private:
    class MutatorASTConsumer_327 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_327(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Incomplete_Using_Declaration_In_Class_Scope_327.h"

// ========================================================================================================
#define MUT327_OUTPUT 1

void MutatorFrontendAction_327::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *UD = Result.Nodes.getNodeAs<clang::UsingDecl>("UsingDecl")) {
      //Filter nodes in header files
      if (!UD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     UD->getLocation()))
        return;
      //Check if the using declaration is inside a class/struct
      auto *DC = UD->getDeclContext();
      if (!DC || !DC->isRecord())
        return;
      //Get the source code text of target node
      auto sourceRange = UD->getSourceRange();
      if (!sourceRange.isValid())
        return;
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);
      //Find the position of the identifier after '::'
      size_t colonColonPos = originalText.find("::");
      if (colonColonPos == std::string::npos)
        return;
      //Find the end of the identifier (either ';' or whitespace before ';')
      size_t semicolonPos = originalText.find(';', colonColonPos);
      if (semicolonPos == std::string::npos)
        return;
      //Check if there's typename keyword
      bool hasTypename = originalText.find("typename") != std::string::npos;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      if (hasTypename) {
        //Remove identifier after '::' in typename using declaration
        mutatedText = originalText.substr(0, colonColonPos + 2) + ";";
      } else {
        //Remove identifier after '::' in regular using declaration
        mutatedText = originalText.substr(0, colonColonPos + 2) + ";";
      }
      mutatedText = "/*mut327*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutatedText);
    }
}
  
void MutatorFrontendAction_327::MutatorASTConsumer_327::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = usingDecl().bind("UsingDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}