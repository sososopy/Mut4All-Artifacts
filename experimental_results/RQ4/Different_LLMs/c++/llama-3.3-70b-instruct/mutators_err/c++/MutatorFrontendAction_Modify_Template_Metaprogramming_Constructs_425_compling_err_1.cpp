//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Metaprogramming_Constructs_425
 */ 
class MutatorFrontendAction_Modify_Template_Metaprogramming_Constructs_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Modify_Template_Metaprogramming_Constructs_425)

private:
    class MutatorASTConsumer_Modify_Template_Metaprogramming_Constructs_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Modify_Template_Metaprogramming_Constructs_425(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_Template_Metaprogramming_Constructs_425.h"

// ========================================================================================================
#define MODIFY_TEMPLATE_METAPROGRAMMING_CONSTRUCTS_425_OUTPUT 1

void MutatorFrontendAction_Modify_Template_Metaprogramming_Constructs_425::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add an additional template parameter
      declaration.insert(declaration.find("<"), ", int N");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunctions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Modify the constexpr function
      declaration.replace(declaration.find("constexpr int foo() { return 0; }"), 
                          "constexpr int foo() { return 1; }");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_Modify_Template_Metaprogramming_Constructs_425::MutatorASTConsumer_Modify_Template_Metaprogramming_Constructs_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher1 = classTemplateDecl().bind("TemplateDecls");
    DeclarationMatcher matcher2 = functionTemplateDecl().bind("TemplateFunctions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}