//header file
#pragma once
#include "Mutator_base.h"

/**
 * Unparenthesized_Initializer_List_As_Macro_Argument_317
 */ 
class MutatorFrontendAction_317 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(317)

private:
    class MutatorASTConsumer_317 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_317(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> candidateTypes;
        std::vector<const clang::VarDecl*> candidateVariables;
        std::vector<const clang::MacroDefinitionRecord*> candidateMacros;
    };
};

//source file
#include "../include/Unparenthesized_Initializer_List_As_Macro_Argument_317.h"

// ========================================================================================================
#define MUT317_OUTPUT 1

void MutatorFrontendAction_317::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CandidateType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate types that can be initialized with braced lists
      if (MT->isCompleteDefinition() && (MT->isStruct() || MT->isClass())) {
        candidateTypes.push_back(MT);
      }
    }
    else if (auto MT = Result.Nodes.getNodeAs<clang::VarDecl>("CandidateVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate variables for macro arguments
      candidateVariables.push_back(MT);
    }
    else if (auto MT = Result.Nodes.getNodeAs<clang::MacroDefinitionRecord>("CandidateMacro")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate macros
      candidateMacros.push_back(MT);
    }
    else if (auto MT = Result.Nodes.getNodeAs<clang::CallExpr>("MacroCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto callText = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Check if any argument contains a braced initializer list with parentheses
      bool found = false;
      std::string mutatedText = callText;
      for (unsigned i = 0; i < MT->getNumArgs(); ++i) {
        auto arg = MT->getArg(i);
        auto argText = stringutils::rangetoStr(*(Result.SourceManager),
                                               arg->getSourceRange());
        //Look for pattern like (Type{...}) or (Type{...})
        size_t pos = argText.find('(');
        if (pos != std::string::npos) {
          size_t bracePos = argText.find('{', pos);
          if (bracePos != std::string::npos) {
            //Check if closing parenthesis follows the brace
            size_t closePos = argText.find(')', bracePos);
            if (closePos != std::string::npos) {
              //Remove parentheses
              std::string newArg = argText.substr(pos + 1, closePos - pos - 1);
              //Replace the argument in the call
              size_t argStart = callText.find(argText);
              if (argStart != std::string::npos) {
                mutatedText.replace(argStart, argText.length(), newArg);
                found = true;
              }
            }
          }
        }
      }
      if (found) {
        //Apply mutation
        Rewrite.ReplaceText(MT->getSourceRange(), mutatedText);
      }
    }
}