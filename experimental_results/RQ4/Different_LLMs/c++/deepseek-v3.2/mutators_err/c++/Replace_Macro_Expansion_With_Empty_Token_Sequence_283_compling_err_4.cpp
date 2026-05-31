//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Macro_Expansion_With_Empty_Token_Sequence_283
 */ 
class MutatorFrontendAction_283 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(283)

private:
    class MutatorASTConsumer_283 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_283(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::MacroDefinitionRecord*> macroDefs;
        std::vector<const clang::MacroExpansion*> macroExpansions;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT283_OUTPUT 1

void MutatorFrontendAction_283::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::MacroDefinitionRecord>("MacroDef")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      macroDefs.push_back(MD);
    }
    else if (auto *ME = Result.Nodes.getNodeAs<clang::MacroExpansion>("MacroExp")) {
      //Filter nodes in header files
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      macroExpansions.push_back(ME);
    }
    else if (auto *ME = Result.Nodes.getNodeAs<clang::MacroExpansion>("LastMacroExp")) {
      //Filter nodes in header files
      if (!ME || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ME->getLocation()))
        return;
      //Get the source code text of target node
      SourceManager &SM = Rewrite.getSourceMgr();
      SourceLocation lastLoc = ME->getSourceRange().getEnd();
      FileID fileID = SM.getFileID(lastLoc);
      SourceLocation fileEnd = SM.getLocForEndOfFile(fileID);
      if (lastLoc != fileEnd) return;
      //Perform mutation on the source code text by applying string replacement
      std::string macroName = ME->getName()->getName().str();
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ME->getSourceRange(), "");
      //Now we need to find a macro definition to mutate to empty
      for (auto *MD : macroDefs) {
        std::string defName = MD->getName()->getName().str();
        if (defName == macroName) {
          SourceRange defRange = MD->getSourceRange();
          std::string defText = stringutils::rangetoStr(SM, defRange);
          size_t pos = defText.find_last_not_of(" ");
          if (pos != std::string::npos && defText[pos] != ')') {
            //Simple macro without parameters
            size_t spacePos = defText.find(' ', defText.find(defName));
            if (spacePos != std::string::npos) {
              std::string newDef = defText.substr(0, spacePos);
              Rewrite.ReplaceText(defRange, newDef);
            }
          } else {
            //Macro with parameters
            size_t parenPos = defText.find('(', defText.find(defName));
            if (parenPos != std::string::npos) {
              size_t endP2arenPos = defText.find(')', parenPos);
              if (endP2arenPos != std::string::npos) {
                std::string newDef = defText.substr(0, endP2arenPos + 1);
                Rewrite.ReplaceText(defRange, newDef);
              }
            }
          }
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_283::MutatorASTConsumer_283::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = macroDefinitionRecord().bind("MacroDef");
    DeclarationMatcher matcher2 = macroExpansion().bind("MacroExp");
    DeclarationMatcher matcher3 = macroExpansion().bind("LastMacroExp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.matchAST(Context);
}