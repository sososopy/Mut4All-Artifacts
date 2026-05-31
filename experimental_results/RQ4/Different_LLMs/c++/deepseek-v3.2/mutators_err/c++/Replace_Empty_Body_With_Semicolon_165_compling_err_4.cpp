//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Empty_Body_With_Semicolon_165
 */ 
class MutatorFrontendAction_165 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(165)

private:
    class MutatorASTConsumer_165 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_165(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Replace_Empty_Body_With_Semicolon_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::RecordDecl>("RecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Check if it's a struct, union, or class
      if (!RD->isStruct() && !RD->isUnion() && !RD->isClass())
        return;
      //Check if it's a complete definition with empty body
      if (!RD->isCompleteDefinition())
        return;
      //Get the body of the record
      if (const clang::RecordDecl* Def = RD->getDefinition()) {
        if (Def->field_empty() && Def->bases().empty()) {
          //Get the source code text of target node
          auto SourceRange = RD->getSourceRange();
          auto SourceText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
          //Check if the body is truly empty (no whitespace/comments)
          size_t BodyStart = SourceText.find('{');
          size_t BodyEnd = SourceText.find('}', BodyStart);
          if (BodyStart != std::string::npos && BodyEnd != std::string::npos) {
            std::string BodyContent = SourceText.substr(BodyStart + 1, BodyEnd - BodyStart - 1);
            //Trim whitespace
            BodyContent.erase(0, BodyContent.find_first_not_of(" \t\n\r"));
            BodyContent.erase(BodyContent.find_last_not_of(" \t\n\r") + 1);
            if (BodyContent.empty()) {
              //Perform mutation on the source code text by applying string replacement
              std::string MutatedText = SourceText.substr(0, BodyStart) + ";";
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}