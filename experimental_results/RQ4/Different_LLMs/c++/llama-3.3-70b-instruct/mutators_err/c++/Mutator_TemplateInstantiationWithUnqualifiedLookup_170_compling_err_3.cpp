//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationWithUnqualifiedLookup_170
 */ 
class MutatorFrontendAction_TemplateInstantiationWithUnqualifiedLookup_170 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        return std::make_unique<MutatorASTConsumer_TemplateInstantiationWithUnqualifiedLookup_170>(CI.getRewriter());
    }

private:
    class MutatorASTConsumer_TemplateInstantiationWithUnqualifiedLookup_170 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationWithUnqualifiedLookup_170(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationWithUnqualifiedLookup_170.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT170_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationWithUnqualifiedLookup_170::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace qualified lookup with unqualified lookup
      size_t pos = declaration.find("this->");
      if (pos != std::string::npos) {
        declaration.replace(pos, 7, "");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_TemplateInstantiationWithUnqualifiedLookup_170::MutatorASTConsumer_TemplateInstantiationWithUnqualifiedLookup_170::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasParent(cxxRecordDecl(isTemplate())).bind("Methods"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}