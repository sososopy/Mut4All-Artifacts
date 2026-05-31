//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Params_With_Variadic_9
 */ 
class MutatorFrontendAction_9 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(9)

private:
    class MutatorASTConsumer_9 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_9(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const CXXRecordDecl *> modified_classes;
    };
};

//source file
#include "../include/Replace_Template_Params_With_Variadic_9.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/DeclCXX.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<ClassTemplateDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        TemplateParameterList *TPL = CT->getTemplateParameters();
        if (!TPL)
            return;

        bool allTypeParams = true;
        for (unsigned i = 0; i < TPL->size(); ++i) {
            const TemplateParameter *param = TPL->getParam(i);
            if (!isa<TemplateTypeParmDecl>(param)) {
                allTypeParams = false;
                break;
            }
        }

        if (!allTypeParams)
            return;

        SourceLocation startLoc = TPL->getTemplateKeywordLoc();
        SourceLocation endLoc = TPL->getRAngleLoc();

        if (startLoc.isInvalid() || endLoc.isInvalid())
            return;

        std::string replacement = "template<typename... Ts>";
        Rewrite.ReplaceText(CharSourceRange::getRange(startLoc, endLoc), replacement);

        CXXRecordDecl *recordDecl = CT->getTemplatedDecl();
        if (recordDecl && recordDecl->isCompleteDefinition()) {
            modified_classes.push_back(recordDecl);
        }
    }
    else if (auto *MD = Result.Nodes.getNodeAs<CXXMethodDecl>("Method")) {
        if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(MD->getLocation()))
            return;

        DeclContext *DC = MD->getDeclContext();
        if (auto *RD = dyn_cast<CXXRecordDecl>(DC)) {
            if (std::find(modified_classes.begin(), modified_classes.end(), RD) != modified_classes.end()) {
                if (MD->hasBody()) {
                    Stmt *body = MD->getBody();
                    if (!body)
                        return;

                    SourceRange bodyRange = body->getSourceRange();
                    if (bodyRange.isInvalid())
                        return;

                    std::string bodyStr = stringutils::rangetoStr(*Result.SourceManager, bodyRange);
                    size_t openBracePos = bodyStr.find('{');
                    if (openBracePos != std::string::npos) {
                        std::string insertion = "\n    g(Ts{}...);\n";
                        bodyStr.insert(openBracePos + 1, insertion);
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(bodyRange), bodyStr);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = classTemplateDecl(hasTemplateParameters()).bind("ClassTemplate");
    DeclarationMatcher matcher2 = cxxMethodDecl().bind("Method");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}