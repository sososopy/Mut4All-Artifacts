//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Template_Instantiation_328
 */ 
class MutatorFrontendAction_Insert_Template_Instantiation_328 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Insert_Template_Instantiation_328>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Insert_Template_Instantiation_328 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Insert_Template_Instantiation_328(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Template_Instantiation_328.h"

// ========================================================================================================
#define MUTINSERT_TEMPLATE_INSTANTIATION_328_OUTPUT 1

void MutatorFrontendAction_Insert_Template_Instantiation_328::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = llvm::StringRef(stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange()));
      //Perform mutation on the source code text by applying string replacement
      std::string instantiation = MT->getNameAsString() + "<int> instance;";
      instantiation = "/*mut328*/" + instantiation;
      //Replace the original AST node with the mutated one
      Rewrite.InsertTextAfterToken(MT->getEndLoc(), instantiation);
      // Use the declaration variable to avoid the unused variable warning
      llvm::errs() << "Declaration: " << declaration << "\n";
    }
}
  
void MutatorFrontendAction_Insert_Template_Instantiation_328::MutatorASTConsumer_Insert_Template_Instantiation_328::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl().bind("Template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}