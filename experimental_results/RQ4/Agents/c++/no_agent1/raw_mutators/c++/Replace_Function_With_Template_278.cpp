//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Template_278
 */ 
class MutatorFrontendAction_278 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(278)

private:
    class MutatorASTConsumer_278 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_278(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/replace_function_with_template_278.h"

// ========================================================================================================
#define MUT278_OUTPUT 1

void MutatorFrontendAction_278::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->isTemplateInstantiation() || FD->isDeleted() || FD->isDefaulted())
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string templatePrefix = "template<typename T> ";
      std::string templateParam = "T ";
      std::string returnType = FD->getReturnType().getAsString();
      std::string functionName = FD->getNameAsString();
      std::string params = declaration.substr(declaration.find('('), declaration.find(')') - declaration.find('(') + 1);
      std::string body = declaration.substr(declaration.find('{'));

      // Construct the template function
      std::string templateFunction = templatePrefix + returnType + " " + functionName + params + " " + body;

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), templateFunction);
    }
}
  
void MutatorFrontendAction_278::MutatorASTConsumer_278::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}