//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Constructor_in_Struct_with_Deduction_Guide_33
 */ 
class MutatorFrontendAction_33 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(33)

private:
    class MutatorASTConsumer_33 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_33(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Template_Constructor_in_Struct_with_Deduction_Guide_33.h"

// ========================================================================================================
#define MUT33_OUTPUT 1

void MutatorFrontendAction_33::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("struct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (MT->isStruct() && MT->hasDefinition()) {
        if (MT->isTemplated()) {
          auto decls = MT->decls();
          for (auto decl : decls) {
            if (decl->getKind() == Decl::Kind::CXXConstructor) {
              auto ctor = llvm::dyn_cast<CXXConstructorDecl>(decl);
              if (ctor->getNumParams() == 1) {
                auto param = ctor->getParamDecl(0);
                if (param->getType() == llvm::dyn_cast<ClassTemplateSpecializationDecl>(MT)->getTemplateArgs()
                                           ->get(0)
                                           .getArgument()
                                           .getAsType()) {
                  llvm::outs() << "Find target ctor\n";
                  auto ctor_content = stringutils::rangetoStr(
                      *(Result.SourceManager), ctor->getSourceRange());
                  auto pos = ctor_content.find('(');
                  if (pos != string::npos) {
                    ctor_content.insert(pos, "/*mut33*/");
                  }
                  content = stringutils::str_replace_all_distinct(
                      content, ctor_content, "");
                  llvm::outs() << ctor_content << '\n';
                  content = stringutils::str_replace_all_distinct(
                      content, "struct", "struct " + ctor_content);
                }
              }
            }
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_33::MutatorASTConsumer_33::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}