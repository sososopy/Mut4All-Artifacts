//source file
#include "../include/insert_unused_template_instantiation_196.h"

// ========================================================================================================
#define MUT196_OUTPUT 1

void MutatorFrontendAction_196::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody() || FD->isTemplateInstantiation())
            return;
        
        functions.push_back(FD);
    } else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isLambda())
            return;
        
        if (!functions.empty()) {
            auto function = functions.back();
            functions.pop_back();
            
            std::string templateDecl = "template<typename T> struct UnusedTemplate { void dummy() {} };";
            std::string instantiation = "UnusedTemplate<int> unusedInstance;";
            
            std::string classContent = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
            size_t insertPos = classContent.rfind('}');
            if (insertPos != std::string::npos) {
                classContent.insert(insertPos, "\n/*mut196*/" + templateDecl + "\n" + instantiation + "\n");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), classContent);
            }
        }
    }
}

void MutatorFrontendAction_196::MutatorASTConsumer_196::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher functionMatcher = functionDecl().bind("Functions");
    DeclarationMatcher classMatcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}