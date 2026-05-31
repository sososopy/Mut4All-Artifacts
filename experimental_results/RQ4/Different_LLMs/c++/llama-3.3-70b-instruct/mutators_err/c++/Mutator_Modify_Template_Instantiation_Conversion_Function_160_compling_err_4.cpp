//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Template_Instantiation_Conversion_Function_160
 */ 
class MutatorFrontendAction_160 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(160)

private:
    class MutatorASTConsumer_160 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_160(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Modify_Template_Instantiation_Conversion_Function_160.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT160_OUTPUT 1

void MutatorFrontendAction_160::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("ConversionFunctions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the call to the conversion function template with a different form
      // For example, change "0, operator int();" to "0, static_cast<int>(operator int());"
      // or "0, operator decltype(0)();"
      size_t pos = declaration.find("operator");
      if (pos != std::string::npos) {
        declaration.replace(pos, 8, "static_cast<int>(operator");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_160::MutatorASTConsumer_160::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(has(cxxMethodDecl(isConversion())).bind("ConversionFunctions"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}