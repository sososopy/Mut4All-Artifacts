//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Function_Specialization_With_Dependent_Type_500
 */ 
class MutatorFrontendAction_500 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(500)

private:
    class MutatorASTConsumer_500 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_500(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Function_Specialization_With_Dependent_Type_500.h"

// ========================================================================================================
#define MUT500_OUTPUT 1

void MutatorFrontendAction_500::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if ((!MT->isStruct() && !MT->isClass()) || MT->isLambda())
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto decls = MT->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Ordinary) {
          if (decl->getKind() == Decl::Kind::FunctionTemplate) {
            auto func_template = dyn_cast<FunctionTemplateDecl>(decl);
            auto special = func_template->getTemplatedDecl();
            auto func_name = special->getNameAsString();
            llvm::outs() << func_name << '\n';
            auto spec = stringutils::rangetoStr(*(Result.SourceManager),
                                                special->getSourceRange());
            llvm::outs() << spec << '\n';
            //Perform mutation on the source code text by applying string replacement
            spec = "template<>\nX " + spec + "{return {func<decltype(X::a)>()};}";
            spec = "/*mut500*/" + spec;
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(MT->getEndLoc(), 0, spec);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_500::MutatorASTConsumer_500::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}