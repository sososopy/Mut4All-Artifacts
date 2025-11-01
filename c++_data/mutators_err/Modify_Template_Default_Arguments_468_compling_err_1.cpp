//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Default_Arguments_468
 */ 
class MutatorFrontendAction_468 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(468)

private:
    class MutatorASTConsumer_468 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_468(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/modify_template_default_arguments_468.h"

// ========================================================================================================
#define MUT468_OUTPUT 1

void MutatorFrontendAction_468::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      if (auto *TTP = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TD->getTemplateParameters()->getParam(0))) {
        if (TTP->hasDefaultArgument()) {
          //Get the source code text of target node
          auto defaultArg = TTP->getDefaultArgument().getAsType().getAsString();
          std::string newDefaultArg = "decltype([](int x) { return x == 0; })";
          //Perform mutation on the source code text by applying string replacement
          if (defaultArg.find("decltype([](int x)") != std::string::npos) {
            std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), TTP->getSourceRange());
            std::string mutatedText = std::regex_replace(originalText, std::regex(defaultArg), newDefaultArg);
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(TTP->getSourceRange()), mutatedText);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_468::MutatorASTConsumer_468::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl(has(templateTypeParmDecl(hasDefaultArgument()))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

// Mutation Information
void chain_of_thought() {
    // This mutator modifies the default argument of a function template
    // by replacing the default lambda or function with a different function object.
    // The new function object maintains the original parameter types and return types.
}