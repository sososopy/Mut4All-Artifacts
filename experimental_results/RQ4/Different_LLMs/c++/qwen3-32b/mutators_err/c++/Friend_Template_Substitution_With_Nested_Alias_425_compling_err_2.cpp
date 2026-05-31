//header file
#pragma once
#include "Mutator_base.h"

/**
 * Friend_Template_Substitution_With_Nested_Alias_425
 */ 
class MutatorFrontendAction_425 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(425)

private:
    class MutatorASTConsumer_425 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_425(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> processedNamespaces;
    };
};

//source file
#include "../include/Friend_Template_Substitution_With_Nested_Alias_425.h"

// ========================================================================================================
#define MUT425_OUTPUT 1

void MutatorFrontendAction_425::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("recordDecl")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;

        if (recordDecl->isTemplated()) {
            const DeclContext *ctx = recordDecl->getDeclContext();
            const NamespaceDecl *ns = dyn_cast<NamespaceDecl>(ctx);
            std::string nsName = ns ? ns->getNameAsString() : "";

            if (processedNamespaces.find(nsName) == processedNamespaces.end()) {
                std::string aliasDecl = "template <typename T> using Alias = T;";
                if (!nsName.empty()) {
                    aliasDecl = "namespace " + nsName + " {\n" + aliasDecl + "\n}\n";
                }
                SourceLocation insertLoc = recordDecl->getBeginLoc();
                Rewrite.InsertText(insertLoc, aliasDecl, true, true);
                processedNamespaces.insert(nsName);
            }

            const TemplateParameterList *params = recordDecl->getTemplateParameters();
            if (!params || params->size() == 0)
                return;
            const NamedDecl *param = params->getParam(0);
            std::string paramName = param->getNameAsString();

            for (const auto *decl : recordDecl->decls()) {
                if (const FriendDecl *friendDecl = dyn_cast<FriendDecl>(decl)) {
                    if (const auto *friendD = friendDecl->getFriendDecl()) {
                        if (const FunctionDecl *funcDecl = dyn_cast<FunctionDecl>(friendD)) {
                            SourceRange range = funcDecl->getSourceRange();
                            if (range.isInvalid())
                                continue;
                            std::string funcDeclStr = stringutils::rangetoStr(*Result.SourceManager, range);

                            std::string aliasInstantiation;
                            if (!nsName.empty()) {
                                aliasInstantiation = nsName + "::Alias<" + paramName + ">";
                            } else {
                                aliasInstantiation = "Alias<" + paramName + ">";
                            }
                            std::string replacedStr = funcDeclStr;
                            size_t pos = 0;
                            while ((pos = replacedStr.find(paramName, pos)) != std::string::npos) {
                                replacedStr.replace(pos, paramName.length(), aliasInstantiation);
                                pos += aliasInstantiation.length();
                            }

                            Rewrite.ReplaceText(range, replacedStr);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_425::MutatorASTConsumer_425::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = clang::ast_matchers::cxxRecordDecl(
        clang::ast_matchers::isTemplated(), 
        clang::ast_matchers::has(
            clang::ast_matchers::friendDecl(
                clang::ast_matchers::hasFunctionDecl().bind("funcDecl")
            )
        )
    ).bind("recordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}