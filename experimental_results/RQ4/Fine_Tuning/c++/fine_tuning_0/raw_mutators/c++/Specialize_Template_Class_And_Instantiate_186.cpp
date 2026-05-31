//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Template_Class_And_Instantiate_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Specialize_Template_Class_And_Instantiate_186.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isTemplated())
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      auto decls = MT->decls();
      bool has_member_template = false;
      for (auto decl : decls) {
        if (decl->isTemplated()) {
          has_member_template = true;
          break;
        }
      }
      if (!has_member_template)
        return;
      content = "/*mut186*/" + content;
      //Perform mutation on the source code text by applying string replacement
      content +=
          "\ntemplate<> class " + MT->getNameAsString() + "<int> {\n};\n";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}