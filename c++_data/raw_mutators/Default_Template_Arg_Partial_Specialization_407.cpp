//header file
#pragma once
#include "Mutator_base.h"

/**
 * Default_Template_Arg_Partial_Specialization_407
 */ 
class MutatorFrontendAction_407 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(407)

private:
    class MutatorASTConsumer_407 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_407(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Default_Template_Arg_Partial_Specialization_407.h"

// ========================================================================================================
#define MUT407_OUTPUT 1

void MutatorFrontendAction_407::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;

      // Get the source code text of target node
      auto templateDeclStr = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());

      // Perform mutation on the source code text by applying string replacement
      std::string newSpecialization = "template <typename T>\nclass " + TD->getNameAsString() +
                                      "<T, typename std::enable_if<std::is_integral<T>::value>::type> {};";

      std::string newDefaultArg = "template <typename T, typename U = typename std::conditional<std::is_pointer<T>::value, T, int>::type>\nclass " + TD->getNameAsString() + ";";

      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), newSpecialization + "\n" + newDefaultArg);
    }
}
  
void MutatorFrontendAction_407::MutatorASTConsumer_407::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = classTemplateDecl(has(templateTypeParmDecl(hasDefaultArgument()))).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}