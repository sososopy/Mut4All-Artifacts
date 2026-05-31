//header file
#pragma once
#include "Mutator_base.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceLocation.h"

/**
 * Replace_Template_Argument_With_Member_Variable_465
 */ 
class MutatorFrontendAction_465 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(465)

private:
    class MutatorASTConsumer_465 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_465(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Decl *> cur_decls;
        std::vector<const clang::TemplateArgument *> cur_template_args;
    };
};

//source file
#include "../include/Replace_Template_Argument_With_Member_Variable_465.h"

// ========================================================================================================
#define MUT465_OUTPUT 1

void MutatorFrontendAction_465::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateDecl")) {
        //Filter nodes in header files
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
            return;
        //Get the source code text of target node
        auto content = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
        //Record the node information of the template class
        cur_decls.push_back(TD);
    } else if (auto *TA = Result.Nodes.getNodeAs<clang::TemplateArgument>("TemplateArg")) {
        //Filter nodes in header files
        if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getAsExpr()->getBeginLoc()))
            return;
        //Record the node information of the template argument
        cur_template_args.push_back(TA);
        //Perform mutation on the source code text by applying string replacement
        if (!cur_decls.empty() && !cur_template_args.empty()) {
            auto template_class = cur_decls.back();
            auto template_arg = cur_template_args.back();
            // Find a static constexpr member variable in the template class
            for (const auto &decl : template_class->decls()) {
                if (const auto *VD = dyn_cast<VarDecl>(decl)) {
                    if (VD->isStatic() && VD->hasAttr<clang::Attr>(clang::attr::Kind::Const)) {
                        // Replace the template argument with the member variable
                        std::string replacement = template_class->getName().getAsString() + "::" + VD->getName().getAsString();
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(template_arg->getAsExpr()->getSourceRange()), replacement);
                        break;
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_465::MutatorASTConsumer_465::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    //Find the template class
    DeclarationMatcher matcher1 = classTemplateDecl().bind("TemplateDecl");
    //Find the template argument
    DeclarationMatcher matcher2 = decl(decl(has(templateArgument())).bind("TemplateArg"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}