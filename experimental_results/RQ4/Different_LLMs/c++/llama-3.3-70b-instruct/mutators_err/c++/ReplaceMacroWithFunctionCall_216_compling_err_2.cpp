//header file
#pragma once
#include "Mutator_base.h"

/**
 * ReplaceMacroWithFunctionCall_216
 */ 
class MutatorFrontendAction_216 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(216)

private:
    class MutatorASTConsumer_216 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_216(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/ReplaceMacroWithFunctionCall_216.h"

// ========================================================================================================
#define REPLACE_MACRO_WITH_FUNCTION_CALL_216_OUTPUT 1

void MutatorFrontendAction_216::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MacroDefinition>("MacroDefinition")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getDefinitionLoc()))
        return;
      //Get the source code text of target node
      auto macroDefinition = stringutils::rangetoStr(*(Result.SourceManager),
                                                     MT->getDefinitionLoc());
      //Perform mutation on the source code text by applying string replacement
      // Replace the macro definition with a function definition
      std::string functionName = MT->getNameAsString() + "_func";
      std::string functionDefinition = "inline " + MT->getNameAsString() + "_func(";
      for (auto param : MT->getMacroInfo()->getParams()) {
        functionDefinition += param->getNameAsString() + ", ";
      }
      functionDefinition = functionDefinition.substr(0, functionDefinition.size() - 2) + ") {\n";
      functionDefinition += macroDefinition.substr(macroDefinition.find_first_of("(") + 1);
      functionDefinition = functionDefinition.substr(0, functionDefinition.find_last_of(")")) + "\n}";
      // Replace all occurrences of the macro with a call to the new function
      auto functionCall = functionName + "(";
      for (auto param : MT->getMacroInfo()->getParams()) {
        functionCall += param->getNameAsString() + ", ";
      }
      functionCall = functionCall.substr(0, functionCall.size() - 2) + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getDefinitionLoc()), functionDefinition);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getDefinitionLoc()), functionCall);
    }
}
  
void MutatorFrontendAction_216::MutatorASTConsumer_216::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = macroDefinition().bind("MacroDefinition");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}