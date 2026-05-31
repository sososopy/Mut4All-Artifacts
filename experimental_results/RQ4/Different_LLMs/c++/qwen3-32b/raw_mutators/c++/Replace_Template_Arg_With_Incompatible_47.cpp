//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Arg_With_Incompatible_47
 */ 
class MutatorFrontendAction_47 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(47)

private:
    class MutatorASTConsumer_47 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_47(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> availableTypes;
    };
};

//source file
#include "../include/Replace_Template_Arg_With_Incompatible_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        availableTypes.push_back(RD->getNameAsString());
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("constructExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getBeginLoc()))
            return;

        clang::QualType constructedType = CE->getType();
        clang::Type *typePtr = constructedType.getTypePtr();
        if (auto *TST = dyn_cast<clang::TemplateSpecializationType>(typePtr)) {
            clang::TemplateName templateName = TST->getTemplateName();
            if (auto *TD = templateName.getAsTemplateDecl()) {
                std::string templateNameStr = TD->getNameAsString();
                if (templateNameStr == "tuple" || templateNameStr == "variant") {
                    clang::DeclContext *DC = TD->getDeclContext();
                    if (auto *ND = dyn_cast<clang::NamespaceDecl>(DC)) {
                        if (ND->getNameAsString() == "std") {
                            clang::TemplateArgumentListInfo *args = TST->getArgs();
                            std::vector<std::string> currentArgNames;
                            for (const auto &arg : args->asArray()) {
                                if (arg.getKind() == clang::TemplateArgument::Type) {
                                    clang::QualType argType = arg.getAsType();
                                    currentArgNames.push_back(argType.getAsString());
                                }
                            }
                            std::string replacementType;
                            for (const std::string &typeName : availableTypes) {
                                if (std::find(currentArgNames.begin(), currentArgNames.end(), typeName) == currentArgNames.end()) {
                                    replacementType = typeName;
                                    break;
                                }
                            }
                            if (!replacementType.empty()) {
                                SourceLocation startLoc = CE->getBeginLoc();
                                SourceLocation endLoc = CE->getEndLoc();
                                SourceManager &SM = *Result.SourceManager;
                                CharSourceRange range = CharSourceRange::getTokenRange(SourceRange(startLoc, endLoc));
                                std::string originalText = Rewrite.getRewrittenText(range);
                                size_t lastCommaPos = originalText.rfind(',');
                                if (lastCommaPos != std::string::npos) {
                                    std::string modifiedText = originalText.substr(0, lastCommaPos) + " " + replacementType + ">";
                                    Rewrite.ReplaceText(range, modifiedText);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("recordDecl");
    DeclarationMatcher constructMatcher = cxxConstructExpr(
        hasType(qualType(
            hasCanonicalType(templateSpecializationType(
                hasName("std::tuple") || hasName("std::variant")))))).bind("constructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.addMatcher(constructMatcher, &callback);
    matchFinder.matchAST(Context);
}