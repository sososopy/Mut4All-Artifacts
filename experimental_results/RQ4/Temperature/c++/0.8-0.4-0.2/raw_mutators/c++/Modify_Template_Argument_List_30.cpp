//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Argument_List_30
 */ 
class MutatorFrontendAction_30 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(30)

private:
    class MutatorASTConsumer_30 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_30(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> existingTypes;
    };
};

//source file
#include "../include/modify_template_argument_list_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
        if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(TA->getLocation()))
            return;

        auto typeStr = TA->getUnderlyingType().getAsString();
        if (typeStr.find("std::variant") != std::string::npos) {
            size_t startPos = typeStr.find('<');
            size_t endPos = typeStr.rfind('>');
            if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos) {
                std::string templateArgs = typeStr.substr(startPos + 1, endPos - startPos - 1);
                std::vector<std::string> types;
                size_t pos = 0;
                while ((pos = templateArgs.find(',')) != std::string::npos) {
                    types.push_back(templateArgs.substr(0, pos));
                    templateArgs.erase(0, pos + 1);
                }
                types.push_back(templateArgs);

                if (!existingTypes.empty()) {
                    int randomIndex = getrandom::getRandomIndex(types.size());
                    int randomTypeIndex = getrandom::getRandomIndex(existingTypes.size());
                    types[randomIndex] = existingTypes[randomTypeIndex];

                    std::string newTypeStr = "std::variant<";
                    for (size_t i = 0; i < types.size(); ++i) {
                        newTypeStr += types[i];
                        if (i < types.size() - 1) {
                            newTypeStr += ", ";
                        }
                    }
                    newTypeStr += ">";

                    Rewrite.ReplaceText(TA->getSourceRange(), newTypeStr);
                }
            }
        }
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        existingTypes.push_back(RD->getNameAsString());
    }
}
  
void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher typeAliasMatcher = typeAliasDecl().bind("TypeAlias");
    DeclarationMatcher recordDeclMatcher = cxxRecordDecl().bind("RecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(typeAliasMatcher, &callback);
    matchFinder.addMatcher(recordDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}