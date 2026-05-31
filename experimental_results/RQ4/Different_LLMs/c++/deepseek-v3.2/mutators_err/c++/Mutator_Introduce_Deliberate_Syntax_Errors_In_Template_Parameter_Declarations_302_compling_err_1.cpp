//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Introduce_Deliberate_Syntax_Errors_In_Template_Parameter_Declarations_302
 */ 
class MutatorFrontendAction_302 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(302)

private:
    class MutatorASTConsumer_302 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_302(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Introduce_Deliberate_Syntax_Errors_In_Template_Parameter_Declarations_302.h"

// ========================================================================================================
#define MUT302_OUTPUT 1

void MutatorFrontendAction_302::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateTypeParm")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Get the source code text of target node
      if (!TP->isParameterPack())
        return;
      SourceRange range = TP->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        // Remove ellipsis entirely
        mutatedText = originalText;
        size_t ellipsisPos = mutatedText.find("...");
        if (ellipsisPos != std::string::npos) {
          mutatedText.erase(ellipsisPos, 3);
        }
      } else if (choice == 1) {
        // Place ellipsis incorrectly (before the keyword)
        mutatedText = originalText;
        size_t ellipsisPos = mutatedText.find("...");
        size_t typePos = mutatedText.find("typename");
        if (typePos != std::string::npos && ellipsisPos != std::string::npos && ellipsisPos > typePos) {
          mutatedText.erase(ellipsisPos, 3);
          mutatedText.insert(typePos, "...");
        } else {
          typePos = mutatedText.find("class");
          if (typePos != std::string::npos && ellipsisPos != std::string::npos && ellipsisPos > typePos) {
            mutatedText.erase(ellipsisPos, 3);
            mutatedText.insert(typePos, "...");
          }
        }
      } else if (choice == 2) {
        // Remove parameter name after ellipsis
        mutatedText = originalText;
        size_t ellipsisPos = mutatedText.find("...");
        if (ellipsisPos != std::string::npos) {
          size_t nameStart = ellipsisPos + 3;
          while (nameStart < mutatedText.size() && mutatedText[nameStart] == ' ') {
            nameStart++;
          }
          size_t nameEnd = nameStart;
          while (nameEnd < mutatedText.size() && mutatedText[nameEnd] != ' ' && mutatedText[nameEnd] != ',' && mutatedText[nameEnd] != '>') {
            nameEnd++;
          }
          if (nameEnd > nameStart) {
            mutatedText.erase(nameStart, nameEnd - nameStart);
          }
        }
      }
      mutatedText = "/*mut302*/" + mutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(range), mutatedText);
    }
}
  
void MutatorFrontendAction_302::MutatorASTConsumer_302::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateTypeParmDecl(isParameterPack()).bind("TemplateTypeParm");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}