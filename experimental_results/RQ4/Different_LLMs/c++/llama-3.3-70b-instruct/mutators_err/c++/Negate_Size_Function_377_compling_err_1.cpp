//header file
#pragma once
#include "Mutator_base.h"

/**
 * Negate_Size_Function_377
 */ 
class MutatorFrontendAction_NegateSizeFunction_377 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(NegateSizeFunction_377)

private:
    class MutatorASTConsumer_NegateSizeFunction_377 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_NegateSizeFunction_377(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/NegateSizeFunction_377.h"

// ========================================================================================================
#define NEGATESIZEFUNCTION_377_OUTPUT 1

void MutatorFrontendAction_NegateSizeFunction_377::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("SizeFunctions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (declaration.find("return") != string::npos) {
        size_t returnPos = declaration.find("return");
        size_t valueStart = declaration.find_first_not_of(" \t", returnPos + 6);
        size_t valueEnd = declaration.find_first_of(" \t;", valueStart);
        string value = declaration.substr(valueStart, valueEnd - valueStart);
        if (value.find("-") == string::npos) {
          declaration.replace(valueStart, value.length(), "-" + value);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_NegateSizeFunction_377::MutatorASTConsumer_NegateSizeFunction_377::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasName("size")).bind("SizeFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}