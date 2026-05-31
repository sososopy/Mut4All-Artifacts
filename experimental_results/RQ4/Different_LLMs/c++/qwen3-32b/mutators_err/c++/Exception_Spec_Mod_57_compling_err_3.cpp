//header file
#pragma once
#include "Mutator_base.h"

/**
 * Exception_Spec_Mod_57
 */ 
class MutatorFrontendAction_57 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(57)
private:
    class MutatorASTConsumer_57 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_57(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<std::string> templateParams;
    };
};

//source file
#include "../include/Mutator_Exception_Spec_Mod_57.h"

// ========================================================================================================
#define MUT57_OUTPUT 1

void MutatorFrontendAction_57::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        // Collect template parameters from the function template
        templateParams.clear();
        if (auto *FTD = FD->getDescribedFunctionTemplate()) {
            for (const auto *Param : *FTD->getTemplateParameters()) {
                if (auto *TTP = dyn_cast<clang::TemplateTypeParmDecl>(Param)) {
                    templateParams.push_back(TTP->getNameAsString());
                }
            }
        }

        if (templateParams.empty()) return;

        // Traverse the function body to find lambdas
        if (auto *Body = FD->getBody()) {
            for (const auto *Stmt : Body->children()) {
                if (auto *Lambda = dyn_cast<clang::LambdaExpr>(Stmt)) {
                    auto &SM = Result.Context->getSourceManager();
                    auto Loc = Lambda->getBeginLoc();
                    if (SM.isInMainFile(Loc)) {
                        std::string LambdaText = stringutils::rangetoStr(SM, Lambda->getSourceRange());
                        std::string NewLambdaText = LambdaText;

                        // Get exception spec type from lambda's type
                        if (auto *FT = Lambda->getType()->getAs<clang::FunctionProtoType>()) {
                            clang::FunctionProtoType::ExceptionSpecType EST = FT->getExceptionSpecType();
                            
                            if (EST == clang::FunctionProtoType::EST_None) {
                                // Add noexcept(false) or throw(T)
                                std::string SelectedParam = templateParams[0];
                                std::string NewSpec = "noexcept(false)";
                                if (getrandom::getRandomIndex(2) == 1) {
                                    NewSpec = "throw(" + SelectedParam + ")";
                                }
                                size_t ClosePos = NewLambdaText.find(")");
                                if (ClosePos != std::string::npos) {
                                    NewLambdaText.insert(ClosePos, " " + NewSpec);
                                } else {
                                    NewLambdaText += " " + NewSpec;
                                }
                            } else if (EST == clang::FunctionProtoType::EST_DynamicNone) {
                                // Replace with throw(T)
                                std::string SelectedParam = templateParams[0];
                                std::string NewSpec = "throw(" + SelectedParam + ")";
                                size_t ExceptPos = NewLambdaText.find("throw(");
                                if (ExceptPos != std::string::npos) {
                                    NewLambdaText.replace(ExceptPos, NewLambdaText.find(")") - ExceptPos + 1, NewSpec);
                                }
                            } else if (EST == clang::FunctionProtoType::EST_BasicNoexcept) {
                                // Replace with noexcept(false)
                                size_t ExceptPos = NewLambdaText.find("noexcept");
                                if (ExceptPos != std::string::npos) {
                                    NewLambdaText.replace(ExceptPos, 8, "noexcept(false)");
                                }
                            } else if (EST == clang::FunctionProtoType::EST_NoexceptFalse) {
                                // Replace with noexcept(true)
                                size_t ExceptPos = NewLambdaText.find("noexcept(false)");
                                if (ExceptPos != std::string::npos) {
                                    NewLambdaText.replace(ExceptPos, 15, "noexcept(true)");
                                }
                            } else if (EST == clang::FunctionProtoType::EST_NoexceptTrue) {
                                // Replace with noexcept(std::is_same_v<T, int>)
                                std::string SelectedParam = templateParams[0];
                                std::string NewSpec = "noexcept(std::is_same_v<" + SelectedParam + ", int>)";
                                size_t ExceptPos = NewLambdaText.find("noexcept(true)");
                                if (ExceptPos != std::string::npos) {
                                    NewLambdaText.replace(ExceptPos, 14, NewSpec);
                                }
                            }

                            if (NewLambdaText != LambdaText) {
                                Rewrite.ReplaceText(Lambda->getSourceRange(), NewLambdaText);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_57::MutatorASTConsumer_57::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasDescendant(lambdaExpr())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}