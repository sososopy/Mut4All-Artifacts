//header file
#pragma once
#include "Mutator_base.h"

/**
 * Remove_Template_Parameter_Comma_333
 */ 
class MutatorFrontendAction_333 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(333)

private:
    class MutatorASTConsumer_333 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_333(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Remove_Template_Parameter_Comma_333.h"

// ========================================================================================================
#define MUT333_OUTPUT 1

void MutatorFrontendAction_333::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("template")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        auto *TPL = MT->getTemplateParameters();
        if (!TPL || TPL->size() < 2)
            return;

        bool found = false;
        size_t index = 0;
        for (size_t i = 0; i < TPL->size() - 1; ++i) {
            auto param1 = TPL->getParam(i);
            auto param2 = TPL->getParam(i + 1);
            if (!param1->isParameterPack() && param2->isParameterPack()) {
                index = i;
                found = true;
                break;
            }
        }
        if (!found)
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation startLoc = TPL->getSourceRange().getBegin();
        SourceLocation endLoc = TPL->getSourceRange().getEnd();
        std::string paramListStr = stringutils::rangetoStr(SM, SourceRange(startLoc, endLoc));

        std::vector<std::string> parts;
        size_t start = 0;
        size_t end = paramListStr.find(',');
        while (end != std::string::npos) {
            parts.push_back(paramListStr.substr(start, end - start));
            start = end + 1;
            end = paramListStr.find(',', start);
        }
        parts.push_back(paramListStr.substr(start));

        std::string newParamListStr;
        for (size_t i = 0; i < parts.size(); ++i) {
            if (i == 0) {
                newParamListStr += parts[i];
            } else {
                if (i - 1 == index) {
                    newParamListStr += parts[i];
                } else {
                    newParamListStr += ',' + parts[i];
                }
            }
        }

        std::string templateDeclStr = stringutils::rangetoStr(SM, MT->getSourceRange());
        size_t lAnglePos = templateDeclStr.find('<');
        size_t rAnglePos = templateDeclStr.find('>');
        if (lAnglePos == std::string::npos || rAnglePos == std::string::npos)
            return;

        std::string newTemplateDeclStr = templateDeclStr.substr(0, lAnglePos + 1) + newParamListStr + templateDeclStr.substr(rAnglePos);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newTemplateDeclStr);
    }
}

void MutatorFrontendAction_333::MutatorASTConsumer_333::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("template");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}