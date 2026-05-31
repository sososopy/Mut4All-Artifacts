//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;
using namespace clang::ast_matchers;

/**
 * modify_inline_variable_templates_610
 */ 
class MutatorFrontendAction_610 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(610)

private:
    class MutatorASTConsumer_610 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_610(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_inline_variable_templates_610.h"

// ========================================================================================================
#define MUT610_OUTPUT 1

void MutatorFrontendAction_610::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("inlineVarTemplate")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      // Check if the variable is a template and has the 'inline' specifier
      if (VD->isInlineSpecified() && VD->isStaticDataMember()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   VD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        size_t inlinePos = declaration.find("inline");
        if (inlinePos != std::string::npos) {
          declaration.erase(inlinePos, 6); // Remove 'inline' keyword
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_610::MutatorASTConsumer_610::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInlineSpecifier(), isStaticStorageClass(), hasAncestor(cxxRecordDecl())).bind("inlineVarTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}