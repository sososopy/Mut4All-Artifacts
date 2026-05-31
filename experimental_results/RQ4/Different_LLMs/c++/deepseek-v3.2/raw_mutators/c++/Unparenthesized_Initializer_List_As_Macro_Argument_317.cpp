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
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CandidateType")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate types that can be initialized with braced lists
      if (MT->isCompleteDefinition() && (MT->isStruct() || MT->isClass())) {
        candidateTypes.push_back(MT);
      }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::VarDecl>("CandidateVar")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate variables for macro arguments
      candidateVariables.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::MacroDefinitionRecord>("CandidateMacro")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record candidate macros
      candidateMacros.push_back(MT);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("MacroCall")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
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
                break;
              }
            }
          }
        }
      }
      //Perform mutation on the source code text by applying string replacement
      if (found) {
        mutatedText = "/*mut317*/" + mutatedText;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
      }
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::Stmt>("InsertionPoint")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //If no suitable macro call found, insert a new one
      if (candidateTypes.empty() || candidateVariables.empty() || candidateMacros.empty())
        return;
      
      //Select random elements
      size_t typeIdx = getrandom::getRandomIndex(candidateTypes.size() - 1);
      size_t varIdx = getrandom::getRandomIndex(candidateVariables.size() - 1);
      size_t macroIdx = getrandom::getRandomIndex(candidateMacros.size() - 1);
      
      auto type = candidateTypes[typeIdx];
      auto var = candidateVariables[varIdx];
      auto macro = candidateMacros[macroIdx];
      
      //Construct new macro call
      std::string typeName = type->getNameAsString();
      std::string varName = var->getNameAsString();
      std::string macroName = macro->getName()->getName().str();
      
      //Generate random values for initializer list
      int val1 = getrandom::getRandomIndex(10);
      int val2 = getrandom::getRandomIndex(10);
      
      std::string newCall = macroName + "(" + varName + ", " + typeName + "{" + std::to_string(val1) + ", " + std::to_string(val2) + "})";
      newCall = "/*mut317*/" + newCall + ";\n";
      
      //Get insertion location
      SourceLocation insertLoc = MT->getBeginLoc();
      //Replace the original AST node with the mutated one (insert after)
      Rewrite.InsertTextAfterToken(insertLoc, newCall);
    }
}
  
void MutatorFrontendAction_317::MutatorASTConsumer_317::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher typeMatcher = cxxRecordDecl().bind("CandidateType");
    DeclarationMatcher varMatcher = varDecl().bind("CandidateVar");
    DeclarationMatcher macroMatcher = macroDefinitionRecord().bind("CandidateMacro");
    StatementMatcher callMatcher = callExpr().bind("MacroCall");
    StatementMatcher insertMatcher = stmt().bind("InsertionPoint");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(macroMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.addMatcher(insertMatcher, &callback);
    matchFinder.matchAST(Context);
}