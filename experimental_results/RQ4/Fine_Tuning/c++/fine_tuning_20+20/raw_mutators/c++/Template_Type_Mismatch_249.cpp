//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Type_Mismatch_249
 */ 
class MutatorFrontendAction_249 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(249)

private:
    class MutatorASTConsumer_249 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_249(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/template_type_mismatch_249.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>("Templates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (auto *FD = dyn_cast<FunctionTemplateDecl>(MT)) {
        auto s = FD->getTemplatedDecl();
        if (auto *FD = dyn_cast<FunctionDecl>(s)) {
          auto params = FD->parameters();
          for (auto param : params) {
            auto type = param->getType();
            if (type->isPointerType()) {
              llvm::outs() << "pointer" << '\n';
              content = stringutils::strReplace(content, "typename", "int");
              break;
            }
          }
        }
      } else if (auto *CD = dyn_cast<ClassTemplateDecl>(MT)) {
        auto s = CD->getTemplatedDecl();
        if (auto *RD = dyn_cast<CXXRecordDecl>(s)) {
          auto fields = RD->fields();
          for (auto field : fields) {
            auto type = field->getType();
            if (type->isIntegerType()) {
              llvm::outs() << "integer" << '\n';
              content = stringutils::strReplace(content, "typename", "string");
              break;
            }
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}