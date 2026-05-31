//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Enable_If_Condition_243
 */ 
class MutatorFrontendAction_243 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(243)
private:
    class MutatorASTConsumer_243 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_243(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), fooExists(false) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        bool fooExists;
    };
};

//source file
#include "../include/Replace_Enable_If_Condition_243.h"

// ========================================================================================================
#define MUT243_OUTPUT 1

void MutatorFrontendAction_243::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        QualType returnType = FD->getReturnType();
        if (auto typedefType = returnType->getAs<TypedefType>()) {
            const TypedefDecl *typedefDecl = typedefType->getDecl();
            if (const auto *classTemplateSpec = dyn_cast<ClassTemplateSpecializationDecl>(typedefDecl->getDeclContext())) {
                TemplateName templateName = classTemplateSpec->getTemplateName();
                if (const auto *templateDecl = templateName.getAsTemplateDecl()) {
                    if (templateDecl->getNameAsString() == "enable_if") {
                        TemplateArgumentListInfo *args = classTemplateSpec->getTemplateArgs();
                        if (args && args->size() >= 1) {
                            TemplateArgumentLoc firstArg = (*args)[0];
                            SourceRange argRange = firstArg.getSourceRange();
                            std::string argText = stringutils::rangetoStr(*Result.SourceManager, argRange);
                            size_t pos = argText.rfind("::");
                            if (pos != std::string::npos) {
                                std::string className = argText.substr(0, pos);
                                std::string newArg = className + "::undefined_member";
                                Rewrite.ReplaceText(argRange, newArg);
                            }
                        }
                    }
                }
            }
        }

        for (const auto *param : FD->parameters()) {
            QualType paramType = param->getType();
            if (auto typedefType = paramType->getAs<TypedefType>()) {
                const TypedefDecl *typedefDecl = typedefType->getDecl();
                if (const auto *classTemplateSpec = dyn_cast<ClassTemplateSpecializationDecl>(typedefDecl->getDeclContext())) {
                    TemplateName templateName = classTemplateSpec->getTemplateName();
                    if (const auto *templateDecl = templateName.getAsTemplateDecl()) {
                        if (templateDecl->getNameAsString() == "enable_if") {
                            TemplateArgumentListInfo *args = classTemplateSpec->getTemplateArgs();
                            if (args && args->size() >= 1) {
                                TemplateArgumentLoc firstArg = (*args)[0];
                                SourceRange argRange = firstArg.getSourceRange();
                                std::string argText = stringutils::rangetoStr(*Result.SourceManager, argRange);
                                size_t pos = argText.rfind("::");
                                if (pos != std::string::npos) {
                                    std::string className = argText.substr(0, pos);
                                    std::string newArg = className + "::undefined_member";
                                    Rewrite.ReplaceText(argRange, newArg);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("foo")) {
        fooExists = true;
    }
}

void MutatorFrontendAction_243::MutatorASTConsumer_243::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher1 = functionDecl().bind("func");
    DeclarationMatcher matcher2 = cxxRecordDecl(hasName("Foo")).bind("foo");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);

    if (!callback.fooExists) {
        SourceLocation endLoc = Context.getTranslationUnitDecl()->getEndLoc();
        callback.Rewrite.InsertText(endLoc, "\nclass Foo {};\n");
    }
}