//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Metaprogramming_Instantiation_432
 */ 
class MutatorFrontendAction_Modify_Template_Metaprogramming_Instantiation_432 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef File) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Modify_Template_Metaprogramming_Instantiation_432>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Modify_Template_Metaprogramming_Instantiation_432 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Modify_Template_Metaprogramming_Instantiation_432(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Template_Metaprogramming_Instantiation_432.h"

// ========================================================================================================
#define MODIFY_TEMPLATE_METAPROGRAMMING_INSTANTIATION_432_OUTPUT 1

void MutatorFrontendAction_Modify_Template_Metaprogramming_Instantiation_432::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Add a new template parameter
      std::string newTemplateParam = "typename U";
      size_t pos = declaration.find("template <");
      declaration.insert(pos + 9, newTemplateParam + ", ");
      // Add a new constraint
      std::string newConstraint = "requires requires(U u) {{ std::cout << u; }}";
      pos = declaration.find("requires");
      declaration.insert(pos, newConstraint);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_Modify_Template_Metaprogramming_Instantiation_432::MutatorASTConsumer_Modify_Template_Metaprogramming_Instantiation_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}